#pragma once

#include <cstdint>

namespace dm
{
    struct PresetResource
    {
        uint64_t preset_id;
        uint64_t resource_id;
    };

    static_assert(sizeof(PresetResource) == 16, "PresetResource size must be 16 bytes");
}
