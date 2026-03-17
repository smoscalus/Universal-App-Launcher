#pragma once

#include "Resource.h"

using namespace domain_models;

class Resource_repository
{
public:
    uint64_t add();
    void update();
    Resource get_by_id();
    void del();
};
