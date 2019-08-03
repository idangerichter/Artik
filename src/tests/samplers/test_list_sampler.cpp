#include "../../lib/sampling/samplers/list_sampler.hpp"
#include "../testutils/testutils_sample_primitives.inl"
#include <map>

TEST(ListSampler, basic_functionality)
{
  // Consts
  const std::map<size_t, int32_t> MAP = { { 371, 100 }, { 374, 200 }, { 377, 300 } };
  const std::vector<size_t> INDICES = { 371, 374, 377 }; // if only CPP had a feature like `keySet()`...
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(_, _)).Times(0);
  EXPECT_CALL(*primitive, Sample(_, _)).Times(0);

  for (const auto& value : MAP)
  {
    EXPECT_CALL(*primitive, Prepare(Ref(wrapper), value.first)).Times(1).WillOnce(Return()).RetiresOnSaturation();
    EXPECT_CALL(*primitive, Sample(Ref(wrapper), value.first))
      .Times(1)
      .WillOnce(Return(Measurement{ value.first, value.second }))
      .RetiresOnSaturation();
  }
  // Code
  ListSampler sampler(INDICES, 0, 0, primitive);
  sampler.Sample(wrapper, measurements);
  // Assertions
  ASSERT_EQ(sampler.GetRequiredSize(), INDICES.size())
    << "The required size of AverageSampler should be the length of the indices list";
  ASSERT_EQ(measurements.size(), INDICES.size())
    << "The vector's size should be always the length of the indices list after sampling";

  for (const auto& measurement : measurements)
  {
    ASSERT_TRUE(MAP.find(measurement.index) != MAP.end());
    ASSERT_EQ(measurement.time, MAP.at(measurement.index));
  }
}
