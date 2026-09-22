#include "Parsers/StepFileParser.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

// --- OCCT ---
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <Poly_Triangulation.hxx>
#include <STEPControl_Reader.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

namespace constellation
{
    namespace
    {
        // Fraction of a solid's bounding-box diagonal used as the linear
        // deflection when no explicit override is supplied. A heuristic, not
        // a physically derived value — expect to tune this once it's run
        // against real models.
        constexpr double kAutoDeflectionFraction = 0.001; // 0.1%

        // Angular deflection (radians) for BRepMesh_IncrementalMesh. Controls
        // faceting density on curved surfaces independent of overall size.
        // OCCT's own commonly-used default.
        constexpr double kAngularDeflection = 0.5;

        bool HasStepExtension(const std::string& filePath)
        {
            std::filesystem::path path(filePath);
            std::string extension = path.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            return extension == ".step" || extension == ".stp";
        }
    }

    StepFileParser::StepFileParser(double linearDeflection)
        : linearDeflectionOverride_(linearDeflection)
    {
    }

    bool StepFileParser::CanParse(const std::string& filePath) const
    {
        return HasStepExtension(filePath);
    }

    double StepFileParser::ResolveDeflection(const Bnd_Box& boundingBox) const
    {
        if (linearDeflectionOverride_.has_value())
        {
            return linearDeflectionOverride_.value();
        }

        if (boundingBox.IsVoid())
        {
            // Degenerate solid with no extent; fall back to a small
            // absolute value rather than dividing by a meaningless diagonal.
            return kAutoDeflectionFraction;
        }

        double xMin = 0.0, yMin = 0.0, zMin = 0.0;
        double xMax = 0.0, yMax = 0.0, zMax = 0.0;
        boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

        const gp_Pnt corner1(xMin, yMin, zMin);
        const gp_Pnt corner2(xMax, yMax, zMax);
        const double diagonal = corner1.Distance(corner2);

        return diagonal * kAutoDeflectionFraction;
    }

    SurfaceMesh StepFileParser::BuildSurfaceMeshFromSolid(const TopoDS_Solid& solid, const std::string& name) const
    {
        SurfaceMesh mesh;
        mesh.Name = name;

        Bnd_Box boundingBox;
        BRepBndLib::Add(solid, boundingBox);
        const double deflection = ResolveDeflection(boundingBox);

        // Tessellates every face of solid in place; triangulation data is
        // attached to each face and retrieved via BRep_Tool::Triangulation below.
        BRepMesh_IncrementalMesh incrementalMesh(solid, deflection, /*isRelative*/ false, kAngularDeflection);

        for (TopExp_Explorer faceExplorer(solid, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
        {
            const TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

            TopLoc_Location location;
            const auto& triangulation = BRep_Tool::Triangulation(face, location);
            if (triangulation.IsNull())
            {
                continue; // face failed to tessellate; skip rather than fail the whole solid
            }

            const gp_Trsf transform = location.Transformation();
            const bool isReversed = (face.Orientation() == TopAbs_REVERSED);

            // Base index for this face's vertices within the mesh being built,
            // since each face's node indices are local to its own triangulation.
            const uint32_t baseIndex = static_cast<uint32_t>(mesh.Vertices.size());

            for (int i = 1; i <= triangulation->NbNodes(); ++i)
            {
                gp_Pnt point = triangulation->Node(i);
                point.Transform(transform);

                // NOTE: if Vector3 turns out to use float rather than double,
                // narrow explicitly here, e.g.
                //   static_cast<float>(point.X()), etc.
                mesh.Vertices.push_back(Vector3d(point.X(), point.Y(), point.Z()));
            }

            for (int i = 1; i <= triangulation->NbTriangles(); ++i)
            {
                int n1 = 0, n2 = 0, n3 = 0;
                triangulation->Triangle(i).Get(n1, n2, n3);

                // OCCT node indices are 1-based and local to this face;
                // convert to 0-based and offset into the shared vertex array.
                uint32_t i0 = baseIndex + static_cast<uint32_t>(n1 - 1);
                uint32_t i1 = baseIndex + static_cast<uint32_t>(n2 - 1);
                uint32_t i2 = baseIndex + static_cast<uint32_t>(n3 - 1);

                if (isReversed)
                {
                    std::swap(i1, i2); // flip winding to keep normals outward-facing
                }

                mesh.Triangles.push_back({i0, i1, i2});

                const Vector3d& v0 = mesh.Vertices[i0];
                const Vector3d& v1 = mesh.Vertices[i1];
                const Vector3d& v2 = mesh.Vertices[i2];

                const Vector3d edge1 = v1 - v0;
                const Vector3d edge2 = v2 - v0;
                Vector3d normal = Vector3d::Cross(edge1, edge2);
                normal.Normalize();

                mesh.Normals.push_back(normal);
            }
        }

        return mesh;
    }

    ParseResult StepFileParser::Parse(const std::string& filePath)
    {
        ParseResult result;

        STEPControl_Reader reader;
        const IFSelect_ReturnStatus readStatus = reader.ReadFile(filePath.c_str());
        if (readStatus != IFSelect_RetDone)
        {
            result.ErrorMessage = "Failed to read STEP file (malformed or unreadable): " + filePath;
            return result;
        }

        reader.TransferRoots();
        const TopoDS_Shape shape = reader.OneShape();
        if (shape.IsNull())
        {
            result.ErrorMessage = "STEP file transferred no usable geometry: " + filePath;
            return result;
        }

        int solidIndex = 0;
        for (TopExp_Explorer solidExplorer(shape, TopAbs_SOLID); solidExplorer.More(); solidExplorer.Next())
        {
            const TopoDS_Solid solid = TopoDS::Solid(solidExplorer.Current());
            const std::string solidName = "Solid_" + std::to_string(solidIndex++);

            SurfaceMesh mesh = BuildSurfaceMeshFromSolid(solid, solidName);
            if (!mesh.Triangles.empty())
            {
                result.Meshes.push_back(std::move(mesh));
            }
        }

        if (result.Meshes.empty())
        {
            result.ErrorMessage = "STEP file contained no solids that produced a triangulated surface: " + filePath;
            return result;
        }

        result.Success = true;
        return result;
    }
}