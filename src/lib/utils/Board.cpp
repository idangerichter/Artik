#include "Board.hpp"
#include "../low_level/cache_intrinsics.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/**
 * Get the file size in bytes
 */
static std::size_t GetFileSize(const std::string& filename);
/**
 * mmap the given fd at the given offset for the given size and return the address
 */
int8_t* LoadFromFile(int fd, std::size_t offset, std::size_t size);

Board::Board(const std::size_t size)
: boardType(BoardType::ALLOCATED), size(size), array(new int8_t[size])
{
}

Board::Board(void* array, std::size_t size)
: boardType(BoardType::PRE_ALLOCATED), size(size), array((int8_t*)array)
{
}

Board::Board(const std::string& filename, std::size_t offset, std::size_t size)
: Board(open(filename.c_str(), O_RDONLY), offset, size == 0 ? GetFileSize(filename) : size)
{
}


Board::Board(int fd, size_t offset, size_t real_size)
: boardType(BoardType::MAPPED_MEMORY), size(real_size), array(LoadFromFile(fd, offset, real_size))
{
}

Board::~Board()
{
    if (this->array != nullptr)
    {
        switch (boardType)
        {
        case BoardType::MAPPED_MEMORY:
            munmap((void*)this->array, this->size);
            break;
        case BoardType::ALLOCATED:
            delete[](this->array);
            break;
        case BoardType::PRE_ALLOCATED:
            break;
        }
    }
}
int8_t& Board::operator[](const std::size_t index)
{
    return array[index];
}
const int8_t& Board::operator[](const std::size_t index) const
{
    return array[index];
}
int32_t Board::Measure(std::size_t index) const
{
    return ProbeTiming(&array[index]);
}
void Board::Flush(std::size_t index) const
{
    MemoryFlush(&array[index]);
}
void Board::FlushAll() const
{
    for (std::size_t i = 0; i < size; i++)
    {
        Flush(i);
    }
}

void Board::Access(std::size_t index) const
{
    MemoryAccess(&array[index]);
}

void Board::LoadToCache() const
{
    for (std::size_t i = 0; i < size; i++)
    {
        Access(i);
    }
}
// region Utils

std::size_t GetFileSize(const std::string& filename)
{
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 3) throw std::runtime_error("Could not open the given file");
    std::size_t size = lseek(fd, 0, SEEK_END);
    close(fd);
    return size;
}
int8_t* LoadFromFile(const int fd, const std::size_t offset, const std::size_t size)
{
    if (fd < 3) throw std::runtime_error("Could not open the given file");

    auto memory = (int8_t*)mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, offset);
    close(fd);
    if (memory == (void*)-1)
        throw std::runtime_error("Failed to allocated the required memory size");
    return memory;
}
// endregion