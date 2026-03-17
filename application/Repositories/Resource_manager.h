#pragma once

#include "Resource.h"

using namespace domain_models;

class Resource_repository
{
public:
    uint64_t create();
    Resource update();
    Resource get_by_id();
    void remove();
};
