#include "memory_wrapper.hpp"
#include "../low_level/cache_intrinsics.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace
{
// Get the file size in bytes
std::size_t GetFileSize(int fd)
{
    std::size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}
// mmap the given fd at the given offset for the given size and return the address
int8_t* LoadFromFile(int fd, std::size_t offset, std::size_t size)
{
    auto memory = (int8_t*)mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, offset);
    if (memory == (void*)-1)
        throw std::runtime_error("Failed to allocated the required memory size");
    return memory;
}

} // namespace

MemoryWrapper::MemoryWrapper(std::size_t size)
: memory_type(MemoryType::ALLOCATED), size(size), array(new int8_t[size])
{
}

MemoryWrapper::MemoryWrapper(void* array, std::size_t size)
: memory_type(MemoryType::PRE_ALLOCATED), size(size), array((int8_t*)array)
{
}

MemoryWrapper::MemoryWrapper(const std::string& filename, std::size_t offset, std::size_t size)
: memory_type(MemoryType::MAPPED_MEMORY)
{
    int fd = open(filename.c_str(), O_RDONLY);

    if (fd == -1)
    {
        throw std::runtime_error("Could not open the given file");
    }
    if (size == 0) {
        size = GetFileSize(fd);
    }

    this->size = std::min(std::max(fd - offset, 0UL), size);
    this->array = LoadFromFile(fd, offset, size);

    close(fd);
}


MemoryWrapper::~MemoryWrapper()
{
    if (this->array == nullptr) return;

    switch (memory_type)
    {
    case MemoryType::MAPPED_MEMORY:
        munmap((void*)this->array, this->size);
        break;
    case MemoryType::ALLOCATED:
        delete[](this->array);
        break;
    case MemoryType::PRE_ALLOCATED:
        break;
    }
}
int8_t& MemoryWrapper::operator[](const std::size_t index)
{
    return array[index];
}
const int8_t& MemoryWrapper::operator[](const std::size_t index) const
{
    return array[index];
}
int32_t MemoryWrapper::Measure(std::size_t index) const
{
    return Memory::ProbeTiming(&array[index]);
}
void MemoryWrapper::Flush(std::size_t index) const
{
    Memory::MemoryFlush(&array[index]);
}
void MemoryWrapper::FlushAll() const
{
    for (std::size_t i = 0; i < size; ++i)
    {
        Flush(i);
    }
}

void MemoryWrapper::Access(std::size_t index) const
{
    Memory::MemoryAccess(&array[index]);
}

void MemoryWrapper::LoadToCache() const
{
    for (std::size_t i = 0; i < size; ++i)
    {
        Access(i);
    }
}