#pragma once
#include "sampler_primitive.hpp"

// Simple sampler primitive. preparation is no-op.
class SimpleSamplerPrimitive : public SamplerPrimitive
{
};

// Sampler primitive for flush-reload. Preparation is to flush the given index
class FlushSamplerPrimitive : public SamplerPrimitive
{
    public:
    void Prepare(MemoryWrapper& memory, size_t index) const override;
};

// Sampler primitive for Prime-Probe. Preparation is to access the given index
class LoadSamplerPrimitive : public SamplerPrimitive
{
    public:
    void Prepare(MemoryWrapper& memory, size_t index) const override;
};
