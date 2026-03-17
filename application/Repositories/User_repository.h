#pragma once

#include "User.h"

using namespace domain_models;

class User_repository
{
public:
    uint64_t create();
    void update();
    User get_by_id();
    void remove();
};
