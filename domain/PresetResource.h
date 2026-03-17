#pragma once 

#include <cstdint>

struct PresetResource
{
    uint64_t id;
    uint64_t preset_id;
    uint64_t resource_id;
};

static_assert(sizeof(PresetResource) == 24, "PresetResource size must be 24 bytes");
