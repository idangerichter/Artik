#include "../../lib/sampling/samplers/simple_sampler.hpp"
#include "../testutils/testutils_sample_primitives.inl"
#include <chrono>

TEST(SimpleSampler, basic_functionality)
{
  // Consts
  const auto INPUT_INDEX = 371;
  const auto MEASUREMENT = Measurement{ 374, 377 };
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), INPUT_INDEX)).Times(1).WillOnce(Return());
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), INPUT_INDEX)).Times(1).WillOnce(Return(MEASUREMENT));
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
  // Consts
  const auto NUM_ITERATIONS = 77;
  const auto BASE_TIME = 300;
  const size_t BASE_INDEX = 20;
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  int mock_index = 0;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _)).Times(NUM_ITERATIONS).WillRepeatedly(Return());
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _))
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

TEST(SimpleSampler, delay)
{
  // Consts
  const auto DELAY_TIME = 20000000; // in ns
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _)).WillOnce(Return());
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _)).WillOnce(Return(Measurement{}));
  // Code
  SimpleSampler sampler(0, DELAY_TIME, primitive);
  auto start_time = std::chrono::high_resolution_clock::now();
  sampler.Sample(wrapper, measurements);
  auto end_time = std::chrono::high_resolution_clock::now();
  // Assertions
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
  ASSERT_GE(duration, DELAY_TIME) << "Sampling should take more time then the delay";
}