#include <iostream>
#include "../main/utils/intel.h"
#include <cstdlib>
#include <ctime>

const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);

int rand(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void perform_writing(uint64_t array[])
{
	array[rand(0, LENGTH - 1)] = 82;
}

int main(int argc, char *argv[])
{
	srand(static_cast<unsigned int>(time(0)));

	// allocate
	auto array = new uint64_t[LENGTH];
	// print loop addr
	std::cout << "::perform_writing function offset is: " << (void *)&perform_writing << std::endl;
	// loop
	while (1)
	{
		perform_writing(array);
	}
	return 0;
}