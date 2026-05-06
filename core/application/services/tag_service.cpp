#include "tag_service.h"
#include "../mappers/tag_mapper.h"
#include <iostream>

TagService::TagService(Engine::Database& tagDb, Engine::Database& linkDb)
    : _tagTable(tagDb.open_table<dm::Tag>("tags")),
      _linkTable(linkDb.open_table<dm::ResourceTag>("resource_tags")) {}

int TagService::createTag(const DTO::CreateTagRequest& req) {
    try {
        dm::Tag tag = TagMapper::to_domain(req, req.user_id);
        uint64_t newId = _tagTable.insert(tag);
        return static_cast<int>(newId);
    } catch (const std::exception& e) {
        std::cerr << "Tag DB Insert Error: " << e.what() << std::endl;
        return -1;
    }
}

std::vector<DTO::TagDto> TagService::getTagsByUserId(uint64_t userId) {
    std::vector<DTO::TagDto> userTags;
    for (uint64_t i = 1; i <= _tagTable.quantity + 1; ++i) {
        try {
            dm::Tag tag = _tagTable.get(i);
            if (tag.name[0] != '\0' && tag.user_id == userId) {
                userTags.push_back(TagMapper::to_dto(tag, i));
            }
        } catch (...) {
            continue;
        }
    }
    return userTags;
}

void TagService::deleteTag(uint64_t id) {
    try {
        _tagTable.remove(id);
        std::cout << "[DB] Tag " << id << " removed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Tag Delete Error: " << e.what() << std::endl;
    }
}

void TagService::linkTagToResource(uint64_t resourceId, uint64_t tagId) {
    try {
        dm::ResourceTag link;
        link.resource_id = resourceId;
        link.tag_id = tagId;
        _linkTable.insert(link);
        std::cout << "[DB] Linked Tag " << tagId << " to Resource " << resourceId << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Link Error: " << e.what() << std::endl;
    }
}

void TagService::unlinkTagFromResource(uint64_t resourceId, uint64_t tagId) {
    try {
        for (uint64_t i = 1; i <= _linkTable.quantity + 1; ++i) {
            try {
                dm::ResourceTag link = _linkTable.get(i);
                if (link.resource_id == resourceId && link.tag_id == tagId) {
                    _linkTable.remove(i);
                    std::cout << "[DB] Unlinked Tag " << tagId << " from Resource " << resourceId << std::endl;
                    break;
                }
            } catch (...) { continue; }
        }
    } catch (const std::exception& e) {
        std::cerr << "Unlink Error: " << e.what() << std::endl;
    }
}

std::vector<uint64_t> TagService::getTagIdsByResource(uint64_t resourceId) {
    std::vector<uint64_t> tagIds;
    for (uint64_t i = 1; i <= _linkTable.quantity + 1; ++i) {
        try {
            dm::ResourceTag link = _linkTable.get(i);
            if (link.resource_id == resourceId) {
                tagIds.push_back(link.tag_id);
            }
        } catch (...) { continue; }
    }
    return tagIds;
}
