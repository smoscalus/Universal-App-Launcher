#pragma once
#include "../../domain/Tag.h"
#include "../../domain/ResourceTag.h"
#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../dto/Tag.h"
#include <vector>

class TagService {
public:
    TagService(Engine::Database& tagDb, Engine::Database& linkDb);

    int createTag(const DTO::CreateTagRequest& req);
    std::vector<DTO::TagDto> getTagsByUserId(uint64_t userId);
    void deleteTag(uint64_t id);

    void linkTagToResource(uint64_t resourceId, uint64_t tagId);
    void unlinkTagFromResource(uint64_t resourceId, uint64_t tagId);
    std::vector<uint64_t> getTagIdsByResource(uint64_t resourceId);

private:
    Engine::Table<dm::Tag> _tagTable;
    Engine::Table<dm::ResourceTag> _linkTable;
};
