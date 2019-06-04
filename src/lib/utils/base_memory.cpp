#include "base_memory.hpp"
#include <utility>

BaseMemory::BaseMemory(int8_t* array, std::function<void(int8_t*)> destructor)
: array(array), destructor(std::move(destructor))
{
}
BaseMemory::BaseMemory(BaseMemory&& other) noexcept
: array(other.array), destructor(other.destructor)
{
}
BaseMemory& BaseMemory::operator=(BaseMemory&& other) noexcept
{
    array = other.array;
    destructor = other.destructor;
    return *this;
}
BaseMemory::~BaseMemory()
{
    destructor(this->array);
}
