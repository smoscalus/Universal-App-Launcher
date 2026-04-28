#pragma once

#include "Table.h"
#include "../Core/WorkFile.h"
#include "../Core/Id.h"
#include "../Storage/FileStorage.h"

#include <cstring>

namespace Engine
{
    class Database
    {
        Core::WorkFile _workFile;
        Core::Id _id;
        Storage::FileStorage _fileStorage;

    public:
        Database(const char *path) : _workFile(path), _id(_workFile), _fileStorage(_workFile, _id) {};

        template <typename T>
        Table<T> open_table(const char *table_name)
        {
            _workFile.set_size(sizeof(T));

            std::string file_name_s = std::string(table_name) + ".hellnot";
            const char *file_name = file_name_s.c_str();

            if (_workFile.is_exists_file() == 0)
            {
                throw std::runtime_error(std::string("Table file does not exist: ") + file_name);
            }

            return Table<T>(_workFile, _fileStorage, _id);
        }
    };
}
