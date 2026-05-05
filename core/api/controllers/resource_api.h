#pragma once

#include "../../application/services/ResourceService.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

class ResourceController {
private:
    crow::App<crow::CORSHandler>& _app;
    ResourceService& _service;

public:
    ResourceController(crow::App<crow::CORSHandler>& app, ResourceService& service) 
        : _app(app), _service(service) {}

    void setup_routes();
};