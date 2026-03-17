#pragma once 

#include <cstdint>

struct Category
{
    uint64_t id;
    char name[255];
    char description[1024];
    uint64_t user_id;
};

static_assert(sizeof(Category) == 1296, "Category size must be 1296 bytes");
