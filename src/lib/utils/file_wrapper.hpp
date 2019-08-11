#pragma once
#include "../includes.hpp"

///////////////////////////////// File Wrapper /////////////////////////////////
//  A wrapper for a file object in linux.
// 
//  Gives interface to loading to memory and unloading from it.
///////////////////////////////////////////////////////////////////////////////
class FileWrapper
{
public:
  // Construct a file from the given filename
  explicit FileWrapper(const std::string& filename);

  FileWrapper(FileWrapper&& file);

  // Immutable
  FileWrapper(const FileWrapper& file) = delete;
  FileWrapper& operator=(const FileWrapper& file) = delete;
  FileWrapper& operator=(FileWrapper&& file) = delete;

  // Load {size} bytes from the given {offset} from the file to memory using mmap
  Byte* LoadToMemory(size_t offset, size_t size) const;
  // Unload the memory that was loaded using #LoadToMemory
  static bool UnloadFromMemory(Byte* memory, size_t size);

  // The file size
  // Note: It is accurate for the time the file object was constructed.
  size_t GetSize() const;

  // Close the associated file
  ~FileWrapper();

private:
  //-----------------------------------------------------------------
  // Note: size_ depends on fd_, thus fd_ should appear before size_
  //-----------------------------------------------------------------

  // Linux file descriptor for the given file
  int fd_;

  // File size
  const size_t size_;
};