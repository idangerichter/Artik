#include "../../lib/sampling/samplers/simple_sampler.hpp"
#include "../testutils/testutils_sample_primitives.inl"


TEST(SimpleSampler, basic_functionality)
{
  // consts
  const auto INPUT_INDEX = 371;
  const auto MEASUREMENT = Measurement{ 374, 377 };
  // dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive.get(), Prepare(_, _)).Times(1).WillOnce(Return());
  EXPECT_CALL(*primitive.get(), Sample(_, _)).Times(1).WillOnce(Return(MEASUREMENT));
  // Code
  SimpleSampler sampler(INPUT_INDEX, 0, primitive);
  sampler.Sample(wrapper, measurements);
  // Assertions
  ASSERT_EQ(sampler.GetRequiredSize(), 1)
    << "The required size of the SimpleSampler should be always 1";
  ASSERT_EQ(measurements.size(), 1) << "The vector's size should be always 1 after sampling";
  ASSERT_TRUE(measurements[0] == MEASUREMENT);
}

TEST(SimpleSampler, multiple_samples)
{
  // consts
  const auto NUM_ITERATIONS = 77;
  const auto BASE_TIME = 300;
  const size_t BASE_INDEX = 20;
  // dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  int mock_index = 0;
  // Expectations
  EXPECT_CALL(*primitive.get(), Prepare(_, _)).Times(NUM_ITERATIONS).WillRepeatedly(Return());
  EXPECT_CALL(*primitive.get(), Sample(_, _))
    .Times(NUM_ITERATIONS)
    .WillRepeatedly(Invoke([&mock_index](auto& wrapper, auto index) mutable -> Measurement {
      auto res = Measurement{ BASE_INDEX + mock_index, BASE_TIME + mock_index };
      mock_index++;
      return res;
    }));

  // Code
  SimpleSampler sampler(0, 0, primitive);
  for (auto i = 0; i < NUM_ITERATIONS; i++)
  {
    sampler.Sample(wrapper, measurements);
    ASSERT_EQ(measurements.size(), i + 1);

    for (auto j = 0; j <= i; j++)
    {
      ASSERT_TRUE(measurements[j] == (Measurement{ BASE_INDEX + j, BASE_TIME + j }));
    }
  }
}
