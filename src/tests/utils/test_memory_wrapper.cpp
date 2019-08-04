#include "../../lib/utils/memory_wrapper.hpp"
#include <fstream>
#include <gtest/gtest.h>

TEST(MemoryWrapper, test_init_from_array)
{
  int8_t arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  auto length = sizeof(arr) / sizeof(*arr);
  MemoryWrapper mem(arr, length);

  ASSERT_EQ(mem.memory_type_, MemoryType::PRE_ALLOCATED);
  ASSERT_EQ(mem.size_, length);

  for (size_t i = 0; i < length; i++)
  {
    // Check not crashing
    mem.Access(i);
    mem.Measure(i);
    mem.Flush(i);
    mem.FlushAll();
    mem.LoadToCache();
    // Check get and set operators
    ASSERT_EQ(mem[i], i);
    mem[i] = 2 * i;
    ASSERT_EQ(mem[i], 2 * i);
    ASSERT_EQ(arr[i], 2 * i);
  }
}

TEST(MemoryWrapper, test_init_from_size)
{
  auto LENGTH = 100;
  MemoryWrapper mem(LENGTH);

  ASSERT_EQ(mem.memory_type_, MemoryType::ALLOCATED);
  ASSERT_EQ(mem.size_, LENGTH);

  for (size_t i = 0; i < LENGTH; i++)
  {
    // Check not crashing
    mem.Access(i);
    mem.Measure(i);
    mem.Flush(i);
    mem.FlushAll();
    mem.LoadToCache();
    // Check get and set operators
    mem[i] = 2 * i;
    ASSERT_EQ(mem[i], 2 * i);
  }
}

TEST(MemoryWrapper, test_init_empty_size)
{
  MemoryWrapper mem(0);

  ASSERT_EQ(mem.memory_type_, MemoryType::ALLOCATED);
  ASSERT_EQ(mem.size_, 0);

  mem.FlushAll();
  mem.LoadToCache();
}

std::ifstream::pos_type GetFileSize(const char* filename)
{
  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
  return in.tellg();
}


TEST(MemoryWrapper, test_init_from_file)
{
  const auto FILENAME = "/bin/ls";
  auto file_size = GetFileSize(FILENAME);
  MemoryWrapper mem(FILENAME);

  ASSERT_EQ(mem.memory_type_, MemoryType::MAPPED_MEMORY);
  ASSERT_EQ(mem.size_, file_size);


  ASSERT_EQ(mem[0], 0x7F);
  ASSERT_EQ(mem[1], 'E');
  ASSERT_EQ(mem[2], 'L');
  ASSERT_EQ(mem[3], 'F');

  mem.FlushAll();
  mem.LoadToCache();
  for (size_t i = 0; i < file_size; i += (file_size / 1000))
  {
    // Check not crashing
    mem.Access(i);
    mem.Measure(i);
    mem.Flush(i);
  }
}