#include <emscripten/bind.h>
#include <string>
#include <iostream>
#include "DTO/Resource.h"

DTO::CreateResourceResponse createResource(DTO::CreateResourceRequest req) {
    std::cout << "API: Received request to save " << req.name << std::endl;

    DTO::CreateResourceResponse res;
    res.id = 777;
    res.success = true;
    res.message = "Resource saved to HELL NAH successfully";

    return res;
}

EMSCRIPTEN_BINDINGS(launcher_core) {
    
    emscripten::value_object<DTO::CreateResourceRequest>("CreateResourceRequest")
        .field("name", &DTO::CreateResourceRequest::name)
        .field("description", &DTO::CreateResourceRequest::description)
        .field("path", &DTO::CreateResourceRequest::path)
        .field("userId", &DTO::CreateResourceRequest::userId);

    emscripten::value_object<DTO::CreateResourceResponse>("CreateResourceResponse")
        .field("id", &DTO::CreateResourceResponse::id)
        .field("success", &DTO::CreateResourceResponse::success)
        .field("message", &DTO::CreateResourceResponse::message);

    emscripten::function("createResource", &createResource);
}
