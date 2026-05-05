#pragma once

#include "../../application/services/user_service.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

class UserController {
private:
    crow::App<crow::CORSHandler>& _app;
    UserService& _service;

public:
    UserController(crow::App<crow::CORSHandler>& app, UserService& service) 
        : _app(app), _service(service) {}

    void setup_routes();
};
