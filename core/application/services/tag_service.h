#pragma once

#include "../../domain/Tag.h"
#include "../../domain/ResourceTag.h"
#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../../infrastructure/persistence/db_context.h"
#include "../dto/Tag.h"
#include <vector>
#include <memory>

class TagService {
public:
    TagService(std::shared_ptr<DbContext> context);

    int createTag(const DTO::CreateTagRequest& req);
    DTO::TagDto updateTag(uint64_t id, const DTO::CreateTagRequest& req);
    std::vector<DTO::TagDto> getTagsByUserId(uint64_t userId);
    void deleteTag(uint64_t id);

    void linkTagToResource(uint64_t resourceId, uint64_t tagId);
    void unlinkTagFromResource(uint64_t resourceId, uint64_t tagId);
    std::vector<uint64_t> getTagIdsByResource(uint64_t resourceId);

private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::Tag> _tagTable;
    Engine::Table<dm::ResourceTag> _linkTable;
};
