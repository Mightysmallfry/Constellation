# Constellation

Shared toolbox library: math types and containers meant to be copied
wholesale into other projects (Osseus, Aquarius, etc.) rather than
pulled in as a package dependency.

## Developing Constellation itself

This repo builds standalone via CMake + vcpkg (manifest mode) + Ninja,
wrapped in a Makefile:

```
make build   # cmake --preset default, then cmake --build
make test    # builds, then runs the Catch2 suite via ctest
make clean   # removes build/
make rebuild # clean + configure + build
```

Requires `VCPKG_ROOT` set in your environment and `ninja` on `PATH`
(both used by `CMakePresets.json`). The `default` preset turns
`CONSTELLATION_BUILD_TESTS` on, so `catch2` (declared in `vcpkg.json`)
gets pulled in automatically — you don't need to pass any extra flags
for `make test` to work.

## How to drop this into a project

1. Copy the entire `Constellation/` folder into your project, e.g.
   `MyProject/external/Constellation/`.
2. In your project's `CMakeLists.txt`:
   ```cmake
   add_subdirectory(external/Constellation)
   target_link_libraries(MyProject PRIVATE Constellation)
   ```
3. Include headers by their subfolder, e.g. `#include <Math/Vector3.h>`
   or `#include <Containers/SparseSet.h>` — there's no `Constellation/`
   folder under `include/`, so paths stay short.

Because this is a manual copy rather than a submodule, updating a
consumer means re-copying the folder over the old one. Keep an eye on
this README's version note (bottom) if you want to track drift between
projects.

### Building as part of a consuming project

Once vendored, the consuming project's own build takes over — there's
no need to invoke Constellation's Makefile from inside another
project's build. `add_subdirectory` folds `Constellation` straight into
the parent's single `cmake --build` pass.

`CONSTELLATION_BUILD_TESTS` defaults to `OFF`, so unless the consuming
project explicitly sets it, Constellation's `tests/` subdirectory and
its Catch2 dependency are skipped entirely inside that build. Note that
in vcpkg manifest mode, only the manifest at the directory you configure
from is used — a consuming project's own `vcpkg.json` won't see
Constellation's, so if you ever do want
`CONSTELLATION_BUILD_TESTS=ON` from inside a consumer, `catch2` needs to
be added to *that* project's manifest too.

If a consuming project wants to validate Constellation's own test suite
in isolation (e.g. in CI, so a Constellation regression doesn't get
misattributed to the consumer), that's a separate, opt-in step rather
than something `add_subdirectory` gives you for free:

```makefile
.PHONY: constellation-test
constellation-test:
	$(MAKE) -C external/Constellation test
```

This runs Constellation's own isolated build (its own `build/` dir, its
own vcpkg resolve) as a discrete step, separate from the consumer's main
build.

## Layout

```
include/            Public headers, organized by category (Math/, Containers/, ...)
src/                 Implementations (.cpp), mirrors include/ layout
tests/               Catch2 unit tests (built only if CONSTELLATION_BUILD_TESTS=ON)
CMakeLists.txt       Library target definition
CMakePresets.json    "default" preset: Ninja + vcpkg toolchain, tests on
vcpkg.json           Manifest — currently just catch2, for tests
Makefile             configure / build / test / clean / rebuild wrapper
```

## Conventions

- **Namespace**: everything lives in `namespace constellation { ... }` —
  no prefix on type names.
- **Function naming**: PascalCase for free functions and methods
  (`Vector3::Normalize()`, `MathUtils::Clamp()`).
- **Portability**: public headers only use standard-library includes
  available on both Windows and Linux (`<cstdint>`, `<cmath>`, etc.).
  No `<windows.h>`, no POSIX-only headers, in anything under `include/`.
- **Headers declare, .cpp defines.** Even small math operators are
  declared in the header and defined in the matching .cpp file, so it's
  obvious at a glance what's public API vs. implementation. The one
  exception is `SparseSet`, which is a template and so must be fully
  defined in its header — noted in a comment there.

## Modules so far

- `Math/` — `Vector2`, `Vector3`, `MathUtils`
- `Containers/` — `Handle`, `SparseSet` (matches the handle/registry
  pattern used in Osseus: `{index, generation}` pairs)

Add new categories as their own subfolder under both `include/` and
`src/`.