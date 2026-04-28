#pragma once

#include "../Core/WorkFile.h"
#include "../Core/Id.h"

#include <cstdint>

namespace Storage
{
    class FileStorage
    {
        Core::WorkFile &_workFile;
        Core::Id &_id;

    public:
        FileStorage(Core::WorkFile &workFile, Core::Id &id) : _workFile(workFile), _id(id) {};
        uint64_t add_record();
    };
};
