#pragma once
#include "../includes.hpp"

// Wrapper for file
class FileWrapper
{
public:
  // Construct a file from the given filename
  FileWrapper(const std::string& filename);
  
  FileWrapper(FileWrapper&& file);

  // Immutable
  FileWrapper(const FileWrapper& file) = delete;
  FileWrapper& operator= (const FileWrapper& file) = delete;
  FileWrapper& operator= (FileWrapper&& file) = delete;

  // Load {size} bytes from the given {offset} from the file to memory using mmap
  Byte* LoadToMemory(size_t offset, size_t size) const;
  // Unload the memory that was loaded using #LoadToMemory
  static bool UnloadFromMemory(Byte* memory, size_t size);
  // Close the associated file 
  ~FileWrapper();  
  
  // The file size
  // Note: It is accurate for the time the file object was constructed.
  size_t size_;

private:
  // Linux file descriptor for the given file
  int fd_;
};