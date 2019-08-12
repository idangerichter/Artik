#include "../main/utils/intel.h"
#include <iostream>

const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);

void perform_writing(uint64_t array[])
{
  array[5] = 0;
  array[101] = 20;
}

int main(int argc, char* argv[])
{
  // allocate
  auto array = new uint64_t[LENGTH];
  // print loop addr
  std::cout << "::perform_writing function offset is: " << (void*)&perform_writing
            << std::endl;
  // loop
  while (1)
  {
    perform_writing(array);
  }
  return 0;
}