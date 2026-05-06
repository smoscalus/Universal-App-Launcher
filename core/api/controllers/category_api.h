#pragma once

#include "../../application/services/category_service.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

class CategoryController {
private:
    crow::App<crow::CORSHandler>& _app;
    CategoryService& _service;

public:
    CategoryController(crow::App<crow::CORSHandler>& app, CategoryService& service) 
        : _app(app), _service(service) {}

    void setup_routes();
};
