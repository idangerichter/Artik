#include "file_wrapper.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace
{
size_t GetSize(int fd)
{
    if (fd == -1)
    {
        return 0;
    }

    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    return size;
}
} // namespace

FileWrapper::FileWrapper(const std::string& filename)
: fd_(open(filename.c_str(), O_RDONLY)), size_(GetSize(fd_))
{
    if (fd_ == -1) throw std::runtime_error("Could not open the given file");
}

FileWrapper::FileWrapper(FileWrapper&& file) : fd_(file.fd_)
{
    file.fd_ = -1;
}

Byte* FileWrapper::LoadToMemory(size_t offset, size_t size) const
{
    Byte* memory = reinterpret_cast<Byte*>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd_, offset));
    if (memory == reinterpret_cast<Byte*>(-1))
    {
        throw std::runtime_error("Failed to allocated the required memory size");
    }
    return memory;
}

bool FileWrapper::UnloadFromMemory(Byte* memory, size_t size)
{
    return munmap(reinterpret_cast<void*>(memory), size) == 0;
}

FileWrapper::~FileWrapper()
{
    close(fd_);
}