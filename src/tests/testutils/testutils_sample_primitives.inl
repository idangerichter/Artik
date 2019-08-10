#ifndef SAMPLE_PRIMITIVE_IMPL
#define SAMPLE_PRIMITIVE_IMPL

#include "../../lib/sampling/sampler_primitive.hpp"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;
using ::testing::NiceMock;
using ::testing::StrictMock;

namespace testutils
{

class MockPrimitiveSampler : public SamplerPrimitive
{
public:
  MOCK_CONST_METHOD2(Prepare, void(const MemoryWrapper&, size_t));
  MOCK_CONST_METHOD2(Sample, Measurement(const MemoryWrapper&, size_t));
};

static std::shared_ptr<MockPrimitiveSampler> Primitive() {
  return std::make_shared<StrictMock<MockPrimitiveSampler>>();
}
static std::shared_ptr<MockPrimitiveSampler>
PrimitiveFromValues(std::vector<Measurement> values, bool should_loop = true, bool should_overwrite_index = false)
{
  auto mock = std::make_shared<NiceMock<MockPrimitiveSampler>>();
  auto i = std::make_shared<int>(0);
  ON_CALL(*mock, Prepare(_, _)).WillByDefault(Return());
  ON_CALL(*mock, Sample(_, _)).WillByDefault(Invoke([=](auto& wrapper, auto index) mutable -> Measurement {
    if (*i >= values.size())
    {
      if (!should_loop)
      {
        throw std::runtime_error("Used simple primitive mock and consumed all values");
      }
      *i = 0;
    }
    if (should_overwrite_index)
    {
      Measurement original = values[(*i)++];
      return Measurement{ index, original.time };
    }
    return values[(*i)++];
  }));
  return mock;
}

static std::shared_ptr<MockPrimitiveSampler>
PrimitiveFromRawValues(const std::initializer_list<int32_t>& values = {0}, bool should_loop = true)
{
  std::vector<Measurement> measurements;
  for (const auto value : values)
  {
    measurements.push_back(Measurement{ 0, value });
  }
  return PrimitiveFromValues(measurements, should_loop, true);
}

static std::shared_ptr<MockPrimitiveSampler>
PrimitiveFromRawValuesVector(const std::vector<int32_t>& values, bool should_loop = true)
{
  std::vector<Measurement> measurements;
  for (const auto value : values)
  {
    measurements.push_back(Measurement{ 0, value });
  }
  return PrimitiveFromValues(measurements, should_loop, true);
}

} // namespace testutils

#endif