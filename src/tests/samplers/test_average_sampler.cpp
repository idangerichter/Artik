#include "../../lib/sampling/samplers/average_sampler.hpp"
#include "../../lib/sampling/samplers/simple_sampler.hpp"

#include "../testutils/testutils_sample_primitives.inl"
#include <chrono>

using time_stamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

TEST(AverageSampler, basic_functionality)
{
  // Consts
  const auto INPUT_INDEX = 371;
  const auto BASE_TIME = 200;
  const auto TIME_DELTA = 3;
  const auto NUM_ITERATIONS_FOR_AVERAGE = 50;
  const auto AVERAGE = (2 * BASE_TIME + (NUM_ITERATIONS_FOR_AVERAGE - 1) * TIME_DELTA) / 2;
  // Dependencies
  auto primitive = testutils::Primitive();
  auto mock_index = 0;
  auto wrapped_sampler = std::make_unique<SimpleSampler>(INPUT_INDEX, 0, primitive);
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(_, _)).Times(0);
  EXPECT_CALL(*primitive, Sample(_, _)).Times(0);

  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), INPUT_INDEX))
    .Times(NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Return())
    .RetiresOnSaturation();
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), INPUT_INDEX))
    .Times(NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Invoke([&mock_index](auto& mem, auto index) {
      return Measurement{ index, BASE_TIME + (mock_index++) * TIME_DELTA };
    }))
    .RetiresOnSaturation();
  // Code
  AverageSampler sampler(std::move(wrapped_sampler), NUM_ITERATIONS_FOR_AVERAGE, 0);
  sampler.Sample(wrapper, measurements);
  // Assertions
  ASSERT_EQ(sampler.GetRequiredSize(), 1)
    << "AverageSampler should require the same size as the Sampler it wraps";
  ASSERT_EQ(measurements.size(), 1) << "After measuring, the size of measurements must be one";
  ASSERT_EQ(measurements[0].index, INPUT_INDEX);
  ASSERT_EQ(measurements[0].time, AVERAGE) << "Measurement's time must be averages of measurements";
}
TEST(AverageSampler, multiple_samples)
{
  // Consts
  const auto INPUT_INDEX = 371;
  const auto BASE_TIME = 200;
  const auto TIME_DELTA = 3;
  const auto NUM_ITERATIONS_FOR_AVERAGE = 33;
  const auto NUM_ITERATIONS = 50;
  // Dependencies
  auto primitive = testutils::Primitive();
  auto mock_index = 0;
  auto wrapped_sampler = std::make_unique<SimpleSampler>(INPUT_INDEX, 0, primitive);
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(_, _)).Times(0);
  EXPECT_CALL(*primitive, Sample(_, _)).Times(0);

  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), INPUT_INDEX))
    .Times(NUM_ITERATIONS * NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Return())
    .RetiresOnSaturation();
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), INPUT_INDEX))
    .Times(NUM_ITERATIONS * NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Invoke([&mock_index](auto& mem, auto index) {
      return Measurement{ index, BASE_TIME + (mock_index++) * TIME_DELTA };
    }))
    .RetiresOnSaturation();
  // Code
  AverageSampler sampler(std::move(wrapped_sampler), NUM_ITERATIONS_FOR_AVERAGE, 0);
  for (size_t i = 0; i < NUM_ITERATIONS; i++)
  {
    sampler.Sample(wrapper, measurements);
    ASSERT_EQ(measurements.size(), i + 1)
      << "The size of the measurements vector after i+1 rounds should be i+1";
    for (size_t j = 0; j <= i; j++)
    {
      auto average = (2 * (BASE_TIME + j * NUM_ITERATIONS_FOR_AVERAGE * TIME_DELTA) +
                      (NUM_ITERATIONS_FOR_AVERAGE - 1) * TIME_DELTA) /
                     2;
      ASSERT_EQ(measurements[j].time, static_cast<int32_t>(average));
    }
  }
}

TEST(AverageSampler, between_rounds_delay)
{
  // Consts
  const auto DELAY_TIME = 500000; // in ns
  const auto NUM_ITERATIONS_FOR_AVERAGE = 33;
  // Dependencies
  auto primitive = testutils::Primitive();
  auto wrapped_sampler = std::make_unique<SimpleSampler>(0, 0, primitive);
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  std::vector<time_stamp> timestamps;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _))
    .Times(NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Invoke([&timestamps](auto& mem, auto index) {
      timestamps.push_back(std::chrono::high_resolution_clock::now());
    }))
    .RetiresOnSaturation();
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _))
    .Times(NUM_ITERATIONS_FOR_AVERAGE)
    .WillRepeatedly(Return(Measurement{}))
    .RetiresOnSaturation();
  // Code
  AverageSampler sampler(std::move(wrapped_sampler), NUM_ITERATIONS_FOR_AVERAGE, DELAY_TIME);
  sampler.Sample(wrapper, measurements);
  // Assertions
  ASSERT_EQ(timestamps.size(), NUM_ITERATIONS_FOR_AVERAGE);
  for (auto i = 1; i < NUM_ITERATIONS_FOR_AVERAGE; i++) {
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamps[i] - timestamps[i-1]).count();
    ASSERT_GE(duration, DELAY_TIME) << "Time between measures should be at least the given delay";
  }
}