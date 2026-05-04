#include <emscripten/bind.h>
#include "dto/Resource.h"
#include "api/ResourceApi.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(launcher_core) {
    emscripten::value_object<DTO::CreateResourceRequest>("CreateResourceRequest")
        .field("name", &DTO::CreateResourceRequest::name)
        .field("description", &DTO::CreateResourceRequest::description)
        .field("path", &DTO::CreateResourceRequest::path)
        .field("user_id", &DTO::CreateResourceRequest::user_id);
        .field("category_id", &DTO::CreateResourceRequest::category_id);

    emscripten::value_object<DTO::CreateResourceResponse>("CreateResourceResponse")
        .field("id", &DTO::CreateResourceResponse::id)
        .field("success", &DTO::CreateResourceResponse::success)
        .field("message", &DTO::CreateResourceResponse::message);

    emscripten::function("createResource", &createResource);
}
