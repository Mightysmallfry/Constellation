#ifndef CONSTELLATION_SURFACEMESH_H
#define CONSTELLATION_SURFACEMESH_H

#include <vector>
#include <array>
#include <string>
#include <cstdint>

#include "Math/Vector3.h"

namespace constellation
{

    struct SurfaceMesh
    {
        std::vector<Vector3d> Vertices;
        std::vector<std::array<uint32_t, 3>> Triangles; // indices into Vertices
        std::vector<Vector3d> Normals;                  // one per triangle, outward-facing

        std::string Name; // e.g. solid/body name from the source file, for diagnostics
    };
}

#endif