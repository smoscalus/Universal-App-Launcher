#pragma once

#include "../Storage/FileStorage.h"
#include "../Core/WorkFile.h"
#include "../Core/DbHeader.h"
#include "../Core/RecordHeader.h"

#include <fstream>
#include <iostream>

namespace Engine
{
    template <typename T>
    class Table
    {
        Core::WorkFile &_workFile;
        Core::Id &_id;
        Storage::FileStorage &_fileStorage;

    public:
        uint64_t quantity;

        Table(Core::WorkFile &workFile, Storage::FileStorage &fileStorage, Core::Id &id) : _workFile(workFile), _fileStorage(fileStorage), _id(id) 
        {
            quantity = _id.get_id();
        }

        uint64_t insert(T obj)
        {
            const char *path = _workFile.get_path();
            size_t size = _workFile.get_size();

            std::ofstream file(path, std::ios::binary | std::ios::app);

            uint64_t id = _fileStorage.add_record();
            quantity++;
            file.write(reinterpret_cast<const char *>(&obj), size);

            file.close();

            return id;
        }

        T get(int id)
        {
            if (id > _id.get_id())
            {
                throw std::runtime_error("Id is not acthual");
            }

            uint64_t offset = sizeof(Core::DbHeader) + (id - 1) * (sizeof(Core::RecordHeader) + sizeof(T));

            std::fstream file(_workFile.get_path(), std::ios::binary | std::ios::in);
            file.seekg(offset, std::ios::beg);

            Core::RecordHeader header;
            file.read(reinterpret_cast<char *>(&header), sizeof(header));

            if (!file)
            {
                throw std::runtime_error("File is broke");
            }

            if (header.id == 0)
            {
                throw std::runtime_error("Record is broke");
            }

            if (header.isDeleted == 1)
            {
                throw std::runtime_error("Record is deleted");
            }

            T value;
            file.read(reinterpret_cast<char *>(&value), sizeof(T));
            return value;
        }

        void remove(int id)
        {
            uint64_t offset = sizeof(Core::DbHeader) + (id - 1) * (sizeof(Core::RecordHeader) + sizeof(T));

            std::fstream file(_workFile.get_path(), std::ios::binary | std::ios::in | std::ios::out);
            file.seekg(offset, std::ios::beg);

            Core::RecordHeader header;
            file.read(reinterpret_cast<char *>(&header), sizeof(header));

            if (!file)
            {
                throw std::runtime_error("File is broke");
            }

            if (header.id == 0 || header.isDeleted == 1)
            {
                throw std::runtime_error("Record is broke");
            }

            header.isDeleted = 1;
            file.seekp(offset, std::ios::beg);
            file.write(reinterpret_cast<char *>(&header), sizeof(header));
        }

        void clear_database()
        {
            _workFile.create_file();
        }
    };
}
