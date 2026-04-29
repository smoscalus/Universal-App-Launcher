#pragma once

#include <cstdint>

namespace Core
{
    struct RecordHeader
    {
        uint64_t id;
        uint32_t size;
        uint8_t isDeleted;
    };

    static_assert(sizeof(RecordHeader) == 16, "RecordHeader size must be 16 bytes");
}
