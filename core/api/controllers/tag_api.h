#pragma once

#include "../../application/services/tag_service.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

class TagController {
private:
    crow::App<crow::CORSHandler>& _app;
    TagService& _service;

public:
    TagController(crow::App<crow::CORSHandler>& app, TagService& service) 
        : _app(app), _service(service) {}

    void setup_routes();
};
