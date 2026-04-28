#pragma once

#include "../Core/RecordHeader.h"
#include "../Core/DbHeader.h"
#include "../Core/WorkFile.h"

#include <iostream>
#include <fstream>
#include <cstdint>

namespace Core
{
    struct Id
    {
    private:
        Core::WorkFile &_workFile;

        uint64_t id = 0;

    public:
        Id(Core::WorkFile &workFile) : _workFile(workFile)
        {
            const char *path = _workFile.get_path();
            size_t size = _workFile.get_size();

            size_t RecordHeader_size = sizeof(Core::RecordHeader) + size;
            size_t DbHeader_size = sizeof(Core::DbHeader);

            std::ifstream file(path, std::ios::binary);
            if (!file)
                throw std::runtime_error("File is broken");

            file.seekg(0, std::ios::end);

            std::streampos file_size = file.tellg();

            if (file_size == DbHeader_size)
            {
                set_id(1);
            }

            size_t data_size = static_cast<size_t>(file_size) - sizeof(Core::DbHeader);
            size_t count = data_size / RecordHeader_size;

            std::streampos lastOffset = sizeof(Core::DbHeader) + (count - 1) * RecordHeader_size;

            Core::RecordHeader header{};
            file.seekg(lastOffset, std::ios::beg);
            file.read(reinterpret_cast<char *>(&header), sizeof(header));

            set_id(header.id);
        };

        void set_id(uint64_t id);
        uint64_t get_id();
        uint64_t next_id();
    };
}
