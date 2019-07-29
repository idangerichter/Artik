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
size_t GetFileSize(int fd)
{
  if (fd == -1)
  {
    return 0;
  }

  // off_t is signed, and we return signed size from this method
  off_t size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  return size;
}
} // namespace

FileWrapper::FileWrapper(const std::string& filename) :
  fd_(open(filename.c_str(), O_RDONLY)),
  size_(GetFileSize(fd_))
{
  if (fd_ == -1) throw std::runtime_error("Could not open the given file");
}

FileWrapper::FileWrapper(FileWrapper&& file) : fd_(file.fd_), size_(file.size_)
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

size_t FileWrapper::GetSize() const
{
  return size_;
}

FileWrapper::~FileWrapper()
{
  close(fd_);
}