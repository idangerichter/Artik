#pragma once
#include <string>

enum class SurfaceType
{
    MAPPED_MEMORY,
    ALLOCATED,
    PRE_ALLOCATED
};

class AttackSurface
{
    public:
    /**
     * Initiate an attack surface from a file.
     * Loads the array from file, using mmap.
     * @param filename A valid filename
     * @param offset The offset from the beginning of the file to load from. Default: 0
     * @param size How many bytes to read. Default: 0 - all.
     */
    explicit AttackSurface(const std::string& filename, std::size_t offset = 0, std::size_t size = 0);
    /**
     * Initiate an attack surface using an allocated array with the given size
     * @param size The size of the array
     */
    explicit AttackSurface(std::size_t size);

    /**
     * Create an attack surface from already allocated array.
     * Note: The array will not be freed
     * @param array the array
     * @param size The size of the array
     */
    AttackSurface(void* array, std::size_t size);
    /**
     * Free all used memory
     */
    ~AttackSurface();

    int8_t& operator[](std::size_t index);
    const int8_t& operator[](std::size_t index) const;

    /**
     * Measure the amount of clock cycles it takes to get the value at index {index}
     * @param index a valid index
     */
    int32_t Measure(std::size_t index) const;
    /**
     * Flush the given index.
     * @param index a valid index
     */
    void Flush(std::size_t index) const;

    /**
     * Flush the whole array from memory
     */
    void FlushAll() const;

    /**
     * Access the array at the given index
     * @param index a valid index
     */
    void Access(std::size_t index) const;

    /**
     * Load the array to the cache (hopefully)
     */
    void LoadToCache() const;

    /**
     * The type of the surface
     */
    const SurfaceType surface_type;
    std::size_t size;

    private:
    int8_t* array;
};