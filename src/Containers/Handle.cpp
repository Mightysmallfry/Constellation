#include "Containers/Handle.h"

#include <limits>

namespace constellation
{
    const Handle Handle::Invalid = Handle{ std::numeric_limits<uint32_t>::max(), 0 };

    bool Handle::operator==(const Handle& other) const
    {
        return Index == other.Index && Generation == other.Generation;
    }

    bool Handle::operator!=(const Handle& other) const
    {
        return !(*this == other);
    }

    bool Handle::IsValid() const
    {
        return Index != Handle::Invalid.Index;
    }
}
