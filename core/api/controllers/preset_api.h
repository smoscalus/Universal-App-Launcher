#pragma once

#include "../../application/services/preset_service.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

class PresetController {
private:
    crow::App<crow::CORSHandler>& _app;
    PresetService& _service;

public:
    PresetController(crow::App<crow::CORSHandler>& app, PresetService& service) 
        : _app(app), _service(service) {}

    void setup_routes();
};
