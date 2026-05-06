#include "preset_api.h"
#include "../../application/dto/Preset.h"

void PresetController::setup_routes() {

    CROW_ROUTE(_app, "/presets").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        DTO::CreatePresetRequest createReq;
        createReq.name = data["name"].s();
        createReq.description = data.has("description") ? std::string(data["description"].s()) : "";
        createReq.user_id = data.has("user_id") ? data["user_id"].u() : 0;

        int id = _service.createPreset(createReq);
        if (id < 0) return crow::response(500, "DB Error");

        crow::json::wvalue res;
        res["id"] = id;
        res["success"] = true;
        res["message"] = "Preset created successfully";
        
        return crow::response(201, res);
    });

    CROW_ROUTE(_app, "/user/<int>/presets")
    ([this](int userId) {
        auto presets = _service.getPresetsByUserId(static_cast<uint64_t>(userId));
        
        std::vector<crow::json::wvalue> list;
        for (const auto& p : presets) {
            list.push_back(p.to_json());
        }
        return crow::json::wvalue(list);
    });

    CROW_ROUTE(_app, "/presets/<int>/resources/<int>").methods(crow::HTTPMethod::POST)
    ([this](int pId, int resId) {
        _service.addResourceToPreset(static_cast<uint64_t>(pId), static_cast<uint64_t>(resId));
        return crow::response(200, "Resource added to preset");
    });

    CROW_ROUTE(_app, "/presets/<int>/resources/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int pId, int resId) {
        _service.removeResourceFromPreset(static_cast<uint64_t>(pId), static_cast<uint64_t>(resId));
        return crow::response(200, "Resource removed from preset");
    });

    CROW_ROUTE(_app, "/presets/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int id) {
        _service.deletePreset(static_cast<uint64_t>(id));
        return crow::response(200);
    });

    CROW_ROUTE(_app, "/presets/run/<int>").methods(crow::HTTPMethod::POST)
    ([this](int id) {
        _service.runPreset(static_cast<uint64_t>(id));
        return crow::response(200, "Preset execution triggered");
    });
}
