#pragma once

#include <cstdint>

struct Resource
{
    uint64_t id;
    char name[255];    
    char description[1024];
    char path[512];
    uint64_t user_id;
    uint64_t category_id;
};

static_assert(sizeof(Resource) == 1816, "Resource size must be 1816 bytes");
