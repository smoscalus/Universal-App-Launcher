
namespace DTO
{
    struct CreateResourceRequest {
        std::string name;
        std::string description;
        std::string path;
        int user_id;
        int category_id;
    };

    struct CreateResourceResponse {
        uint64_t id;
        bool success;
        std::string message;
    };
}
