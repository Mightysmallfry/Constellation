#ifndef CONSTELLATION_PARSERESULT_H
#define CONSTELLATION_PARSERESULT_H

#include <string>
#include <vector>

#include "Geometry/SurfaceMesh.h"

namespace constellation
{
    // Returned by value from IParser::Parse. Bundles success/data/error into
    // one immutable value rather than relying on exceptions, an out-param,
    // or a separate GetLastError()-style call that's easy to forget to check.
    //
    // Callers must check Success before relying on Meshes; a false Success
    // always means Meshes is empty and ErrorMessage explains why.
    struct ParseResult
    {
        bool Success = false;
        std::string ErrorMessage; // empty when Success is true

        // One SurfaceMesh per solid found in the source file.
        std::vector<SurfaceMesh> Meshes;
    };
}
#endif