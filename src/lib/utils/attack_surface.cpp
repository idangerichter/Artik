#include "attack_surface.hpp"
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
static std::size_t GetFileSize(int fd);
/**
 * mmap the given fd at the given offset for the given size and return the address
 */
int8_t* LoadFromFile(int fd, std::size_t offset, std::size_t size);

AttackSurface::AttackSurface(const std::size_t size)
: surface_type(SurfaceType::ALLOCATED), size(size), array(new int8_t[size])
{
}

AttackSurface::AttackSurface(void* array, std::size_t size)
: surface_type(SurfaceType::PRE_ALLOCATED), size(size), array((int8_t*)array)
{
}

AttackSurface::AttackSurface(const std::string& filename, std::size_t offset, std::size_t size)
: surface_type(SurfaceType::MAPPED_MEMORY)
{
    int fd = open(filename.c_str(), O_RDONLY);

    if (fd == -1)
    {
        throw std::runtime_error("Could not open the given file");
    }

    this->size = std::min(std::max(fd - offset, 0UL), size);
    this->array = LoadFromFile(fd, offset, size);

    close(fd);
}


AttackSurface::~AttackSurface()
{
    if (this->array != nullptr)
    {
        switch (surface_type)
        {
        case SurfaceType ::MAPPED_MEMORY:
            munmap((void*)this->array, this->size);
            break;
        case SurfaceType::ALLOCATED:
            delete[](this->array);
            break;
        case SurfaceType::PRE_ALLOCATED:
            break;
        }
    }
}
int8_t& AttackSurface::operator[](const std::size_t index)
{
    return array[index];
}
const int8_t& AttackSurface::operator[](const std::size_t index) const
{
    return array[index];
}
int32_t AttackSurface::Measure(std::size_t index) const
{
    return Memory::ProbeTiming(&array[index]);
}
void AttackSurface::Flush(std::size_t index) const
{
    Memory::MemoryFlush(&array[index]);
}
void AttackSurface::FlushAll() const
{
    for (std::size_t i = 0; i < size; i++)
    {
        Flush(i);
    }
}

void AttackSurface::Access(std::size_t index) const
{
    Memory::MemoryAccess(&array[index]);
}

void AttackSurface::LoadToCache() const
{
    for (std::size_t i = 0; i < size; i++)
    {
        Access(i);
    }
}
// region Utils

std::size_t GetFileSize(int fd)
{
    std::size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}
int8_t* LoadFromFile(const int fd, const std::size_t offset, const std::size_t size)
{
    auto memory = (int8_t*)mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, offset);
    if (memory == (void*)-1)
        throw std::runtime_error("Failed to allocated the required memory size");
    return memory;
}
// endregion