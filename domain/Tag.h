#pragma once 

#include <cstdint>

struct Tag
{
    uint64_t id;
    char name[255];
    char description[1024];
};

static_assert(sizeof(Tag) == 1288, "Tag size must be 1288 bytes");
