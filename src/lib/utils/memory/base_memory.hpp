#pragma once
#include <functional>

class BaseMemory
{
    public:
    BaseMemory(int8_t* array, std::function<void(int8_t*)> destructor);
    BaseMemory(BaseMemory&& other) noexcept;
    BaseMemory& operator=(BaseMemory&& other) noexcept;

    // copy constructor is disabled
    BaseMemory(const BaseMemory&) = delete;
    // copy assignment is disabled
    BaseMemory& operator=(const BaseMemory& other) = delete;

    // The encapsulated data
    int8_t* array;

    private:
    std::function<void(int8_t*)> destructor;
};
