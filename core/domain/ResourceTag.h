#pragma once

#include <cstdint>

namespace dm
{
    struct ResourceTag
    {
        uint64_t id;
        uint64_t resource_id;
        uint64_t tag_id;
    };

    static_assert(sizeof(ResourceTag) == 24, "ResourceTag size must be 24 bytes");
}
