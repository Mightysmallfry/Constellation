#ifndef CONSTELLATION_STEPFILEPARSER_H
#define CONSTELLATION_STEPFILEPARSER_H

#include <optional>
#include <string>

#include "Interfaces/IParser.h"

// Forward declarations only — StepFileParser.cpp includes the actual OCCT
// headers. Keeping OCCT types out of this header means anything that only
// needs to hold a StepFileParser (or an IParser*) doesn't have to pull in
// OpenCASCADE itself.
class Bnd_Box;
class TopoDS_Solid;

namespace constellation
{
    // Reads .step / .stp files via OpenCASCADE (OCCT):
    //   1. STEPControl_Reader parses the file into a TopoDS_Shape.
    //   2. TopExp_Explorer walks it to find each independent TopAbs_SOLID.
    //   3. BRepMesh_IncrementalMesh tessellates each solid's faces to within
    //      a linear deflection tolerance.
    //   4. Each solid's tessellated faces are extracted into global-space
    //      triangles and packed into one SurfaceMesh.
    //
    // A STEP file may contain more than one solid (an assembly, or incidental
    // separate bodies like rivets/fasteners even in what's conceptually "one
    // part"). Parse() returns one SurfaceMesh per solid found, in file order.
    class StepFileParser : public IParser
    {
    public:
        // Deflection is auto-computed per solid, as a fraction of that
        // solid's own bounding-box diagonal. See ResolveDeflection().
        StepFileParser() = default;

        // Every solid parsed by this instance uses this fixed linear
        // deflection instead of the auto-computed one. Must be > 0.
        explicit StepFileParser(double linearDeflection);

        bool CanParse(const std::string& filePath) const override;
        ParseResult Parse(const std::string& filePath) override;

    private:
        std::optional<double> linearDeflectionOverride_;

        // Resolves the linear deflection to tessellate one solid with:
        // the override if one was supplied at construction, otherwise a
        // fraction of that solid's own bounding-box diagonal. Auto-compute
        // runs per solid (not once for the whole file) so a small part
        // sitting next to a large one in the same file isn't over- or
        // under-tessellated relative to its own size.
        double ResolveDeflection(const Bnd_Box& boundingBox) const;

        // Tessellates one solid (mutating it in place via
        // BRepMesh_IncrementalMesh) and extracts its faces into a single
        // global-space SurfaceMesh.
        SurfaceMesh BuildSurfaceMeshFromSolid(const TopoDS_Solid& solid, const std::string& name) const;
    };
}
#endif