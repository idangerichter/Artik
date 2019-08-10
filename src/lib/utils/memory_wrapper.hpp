#pragma once
#include "../includes.hpp"
#include "file_wrapper.hpp"
#include <memory>

enum class MemoryType
{
  MAPPED_MEMORY,
  ALLOCATED,
  PRE_ALLOCATED
};


//  A generic wrapper for a memory object that can measure time,
//  flush memory, evict so on and so forth.
class MemoryWrapper
{
public:
  // Initiate an memory wrapper from a file.
  // Loads size bytes to the array from file, starting from offset.
  explicit MemoryWrapper(const std::string& filename, size_t offset = 0, size_t size = 0);

  MemoryWrapper(const MemoryWrapper& mem_wrapper) = delete;

  // Initiate an memory wrapper using an allocated array with the given size
  explicit MemoryWrapper(size_t size);

  // Create a memory wrapper from already allocated array.
  // Note: The array will not be freed!!
  explicit MemoryWrapper(void* array, size_t size);
  MemoryWrapper(MemoryWrapper&& mem_wrapper);

  Byte& operator[](size_t index);
  const Byte& operator[](size_t index) const;
  MemoryWrapper& operator=(const MemoryWrapper& mem_wrapper) = delete;

  // Measure the amount of clock cycles it takes to get the value at index
  int32_t Measure(size_t index) const;

  // Flush the given index.
  void Flush(size_t index) const;

  // Flush the whole array from memory
  void FlushAll() const;

  // Access the array at the given index
  void Access(size_t index) const;

  // Load the array to the cache
  void LoadToCache() const;

  // The type of the memory
  const MemoryType memory_type_;
  // the memory size
  size_t size_;

private:
  explicit MemoryWrapper(const FileWrapper&& file, size_t offset, size_t size);

  std::unique_ptr<Byte[], std::function<void(Byte*)>> array_;
};
