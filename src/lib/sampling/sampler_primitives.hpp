#pragma once
#include "sampler_primitive.hpp"

class SimpleSamplerPrimitive : public SamplerPrimitive
{
};

class FlushSamplerPrimitive : public SamplerPrimitive
{
    void Prepare(MemoryWrapper& memory, size_t index) const override;
};
class LoadSamplerPrimitive : public SamplerPrimitive
{
    void Prepare(MemoryWrapper& memory, size_t index) const override;
};
