#include "../dto/Resource.h"

class ResourceApi {
public:
    // POST /resources
    static DTO::CreateResourceResponse post(DTO::CreateResourceRequest req);
    
    // GET /resources/:id
    static DTO::Resource get(int id);
};