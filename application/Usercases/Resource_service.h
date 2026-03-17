#pragma once

#include "Resource.h"

using namespace domain_models;

class Resource_service
{
public:
    void start();
    void remove();
    void rename();
    void change_description();
    void change_path();
};
