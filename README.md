# Artik
Artik is a library for cache side channel attack. It was built as part of a workshop for cryptographic attacks in Tel Aviv University.  
## Requirements
- Linux enviorment x86 intel processor.
- CMAKE 3.5 or higher.
- C++ 14 or higher.
## Goals
While buliding Artik we had the following goals in mind:
- Build a generic cache side channel attack that supports both Flush+Reload, Prime+Probe and should extends easily to future other attacks.
- Keep overhead of the library to minimum. Avoid as must as possible corrupting the cache by unnecessary copying or allocating.
- Offer an easy to use library for attacking cache.
## Compilation
In order to compile Artik, run the following commands:
```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```
After creating build for the first time you may use the same library.
## Tests
We use gtest for unit test and have additional system tests. In order to run 
the unit tests, run  `ctest -V` in the build library.

## Code Example
```cpp
auto indices = { addr0, addr1 };
// Setup
auto primitive = std::make_unique<FlushSamplerPrimitive>();
auto sampler = std::make_unique<ListSampler>(
    indices,
    SAMPLE_MEASURE_DELAY,
    BETWEEN_ITEMS_DELAY,
    std::move(primitive));
auto average_sampler = std::make_unique<AverageSampler>(
    std::move(sampler),
    SAMPLE_ROUNDS,
    BETWEEN_ROUNDS_DELAY);
AttackManager attack(
    MemoryWrapper(path),
    AttackType::FlushReload,
    std::move(average_sampler));
attack.Calibrate();

// Attack
std::vector<Measurement> measurements;
std::vector<AttackResult> results;
attack.Attack(measurements, results);
for (const AttackResult& result : results)
{
    if(result.score < MIN_SCORE) { continue; }
    if(result.index == addr0)
        std::cout << "Option 0!" << std::endl;
    else if(result.index == addr1)
        std::cout << "Option 1!" << std::endl;
}
```