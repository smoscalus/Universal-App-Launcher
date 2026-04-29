#pragma once

#include <cstddef>

namespace Core
{
    struct WorkFile
    {
    private:
        char path[256];
        size_t size;

    public:
        WorkFile(const char *path);

        void create_file();
        void read_file();
        int is_exists_file();

        void set_size(size_t size)
        {
            this->size = size;
        }
        size_t get_size() const
        {
            return this->size;
        }
        const char *get_path() const
        {
            return this->path;
        }
    };
}
