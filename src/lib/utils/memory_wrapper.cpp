#include "memory_wrapper.hpp"
#include "../low_level/cache_intrinsics.hpp"
#include <stdexcept>
#include <sys/mman.h>

MemoryWrapper::MemoryWrapper(size_t size) :
  memory_type_(MemoryType::ALLOCATED),
  size_(size),
  array_(new Byte[size], [](auto array) { delete[] array; })
{
}

MemoryWrapper::MemoryWrapper(void* array, size_t size) :
  memory_type_(MemoryType::PRE_ALLOCATED),
  size_(size),
  array_(reinterpret_cast<Byte*>(array), [](auto array) {})
{
}

MemoryWrapper::MemoryWrapper(const std::string& filename, size_t offset, size_t size) :
  MemoryWrapper(FileWrapper(filename), offset, size)
{
}

MemoryWrapper::MemoryWrapper(const FileWrapper&& file, size_t offset, size_t size) :
  memory_type_(MemoryType::MAPPED_MEMORY),
  size_(size != 0 ? std::min(size, file.GetSize()) : file.GetSize()),
  array_(file.LoadToMemory(offset, size_),
         [this](auto array) { FileWrapper::UnloadFromMemory(array, size_); })
{
}

MemoryWrapper::MemoryWrapper(MemoryWrapper&& mem_wrapper) :
  memory_type_(mem_wrapper.memory_type_),
  size_(mem_wrapper.size_),
  array_(std::move(mem_wrapper.array_))
{
  mem_wrapper.size_ = 0;
}

Byte& MemoryWrapper::operator[](const size_t index)
{
  return array_[index];
}
const Byte& MemoryWrapper::operator[](const size_t index) const
{
  return array_[index];
}
int32_t MemoryWrapper::Measure(size_t index) const
{
  return Memory::ProbeTiming(&array_[index]);
}
void MemoryWrapper::Flush(size_t index) const
{
  Memory::MemoryFlush(&array_[index]);
}
void MemoryWrapper::FlushAll() const
{
  for (size_t i = 0; i < size_; ++i)
  {
    Flush(i);
  }
}

void MemoryWrapper::Access(size_t index) const
{
  Memory::MemoryAccess(&array_[index]);
}

void MemoryWrapper::LoadToCache() const
{
  for (size_t i = 0; i < size_; ++i)
  {
    Access(i);
  }
}