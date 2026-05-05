// #include <string>
// #include <iostream>

// #include "../dto/Resource.h"
// #include "../infrastructure/install/include/Crow/crow_all.h"


// void setup_routes()
// {
//     CROW_ROUTE(_app, "/resource").methods(crow::HTTPMethod::POST)
//     ([this](const crow::request& req) {
//         auto x = crow::json::load(req.body);
//         if (!x) return crow::response(400);

//         crow::json::wvalue res;
//         res["status"] = "created";
//         return crow::response(res);
//     });

//     CROW_ROUTE(_app, "/resource/<int>")
//    ([this](int id) {
//         crow::json::wvalue res;
//         res["id"] = id;
//         res["status"] = "get";
//         return crow::response(res);
//     });
// };
