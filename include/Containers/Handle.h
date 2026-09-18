#ifndef CONSTELLATION_HANDLE_H
#define CONSTELLATION_HANDLE_H

#include <cstdint>

namespace constellation
{
    // A generic {index, generation} handle. Consumers (e.g. a Registry)
    // are responsible for validating the generation against a live slot
    // before dereferencing the index; this type is intentionally a plain
    // value with no validation logic of its own.
    struct Handle
    {
        uint32_t Index = 0;
        uint32_t Generation = 0;

        bool operator==(const Handle& other) const;
        bool operator!=(const Handle& other) const;
        bool IsValid() const;

        static const Handle Invalid;
    };
}

#endif