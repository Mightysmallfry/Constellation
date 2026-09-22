#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "Parsers/StepFileParser.h"
#include "TestFixturesPath.h"

using namespace constellation;
using constellation::test::kStepFixturesDir;

namespace
{
    // Counted via `grep -c MANIFOLD_SOLID_BREP` on the raw STEP text --
    // not verified against an actual OCCT parse. If this test fails on
    // the solid count specifically, re-check that count rather than
    // assuming the parser regressed.
    constexpr size_t kExpectedSolidCount = 47;
}

// Tagged [integration][slow]: this fixture is a real ~50MB SolidWorks
// assembly export, not a minimal synthetic case -- expect this to take
// noticeably longer than the rest of the suite. Run the fast suite alone
// with `ConstellationTests ~[slow]`.
TEST_CASE("StepFileParser parses the G22 aircraft assembly", "[Parsing][StepFileParser][fixture][integration][slow]")
{
    const std::filesystem::path fixture = kStepFixturesDir / "G22.STEP";
    INFO("Looking for STEP fixture at: " << std::filesystem::absolute(fixture).string());
    REQUIRE(std::filesystem::exists(fixture));

    StepFileParser parser;
    ParseResult result = parser.Parse(fixture.string());

    REQUIRE(result.Success);
    INFO("ErrorMessage: " << result.ErrorMessage); // empty on success, useful if this ever fails

    CHECK(result.Meshes.size() == kExpectedSolidCount);

    size_t totalVertices = 0;
    size_t totalTriangles = 0;

    for (const SurfaceMesh& mesh : result.Meshes)
    {
        CAPTURE(mesh.Name);

        CHECK_FALSE(mesh.Vertices.empty());
        CHECK_FALSE(mesh.Triangles.empty());
        CHECK(mesh.Triangles.size() == mesh.Normals.size());

        // Every triangle index has to be a valid offset into this mesh's
        // own Vertices -- catches an indexing bug in BuildSurfaceMeshFromSolid
        // that a bare "non-empty" check wouldn't.
        for (const auto& triangle : mesh.Triangles)
        {
            for (uint32_t index : triangle)
            {
                CHECK(index < mesh.Vertices.size());
            }
        }

        totalVertices += mesh.Vertices.size();
        totalTriangles += mesh.Triangles.size();
    }

    // Sanity bounds on the assembly as a whole, not tied to the exact
    // tessellation the current deflection heuristic happens to produce --
    // wide enough to not break if kAutoDeflectionFraction is retuned later.
    CHECK(totalVertices > 1000);
    CHECK(totalTriangles > 1000);
}

TEST_CASE("StepFileParser CanParse accepts the G22 fixture's extension", "[Parsing][StepFileParser][fixture]")
{
    const std::filesystem::path fixture = kStepFixturesDir / "G22.STEP";
    INFO("Looking for STEP fixture at: " << std::filesystem::absolute(fixture).string());
    REQUIRE(std::filesystem::exists(fixture));

    StepFileParser parser;
    CHECK(parser.CanParse(fixture.string()));
}