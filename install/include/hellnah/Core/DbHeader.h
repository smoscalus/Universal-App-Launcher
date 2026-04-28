#pragma once

#include <cstdint>

namespace Core
{
    struct DbHeader
    {
        char magic[8]{"HELLNAH"};
        uint32_t version;
    };

    static_assert(sizeof(DbHeader) == 12, "DbHeader size must be 12 bytes");
}
