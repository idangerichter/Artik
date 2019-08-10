#include "../../../lib/sampling/samplers/list_sampler.hpp"
#include "../../testutils/testutils_sample_primitives.inl"
#include <chrono>
#include <map>

using time_stamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

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

TEST(ListSampler, multiple_samples)
{
  // Consts
  const int NUM_ITERATIONS = 35;
  const std::vector<size_t> ITEMS = { 1, 2, 3 };
  const auto BASE_TIME = 300;
  const size_t BASE_INDEX = 20;
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  int mock_index = 0;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _))
    .Times(static_cast<int>(NUM_ITERATIONS * ITEMS.size()))
    .WillRepeatedly(Return());
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _))
    .Times(static_cast<int>(NUM_ITERATIONS * ITEMS.size()))
    .WillRepeatedly(Invoke([&mock_index](auto& wrapper, auto index) mutable -> Measurement {
      auto res = Measurement{ BASE_INDEX + mock_index, BASE_TIME + mock_index };
      mock_index++;
      return res;
    }));

  // Code
  ListSampler sampler(ITEMS, 0, 0, primitive);
  for (auto i = 0; i < NUM_ITERATIONS; i++)
  {
    sampler.Sample(wrapper, measurements);
    ASSERT_EQ(measurements.size(), (i + 1) * ITEMS.size());

    for (size_t j = 0; j < (i + 1) * ITEMS.size(); j++)
    {
      ASSERT_TRUE(measurements[j] == (Measurement{ BASE_INDEX + j, static_cast<int32_t>(BASE_TIME + j) }));
    }
  }
}

TEST(ListSampler, sample_measure_delay)
{
  // Consts
  const auto DELAY_TIME = 5000000; // in ns
  const std::vector<size_t> INDICES = { 1, 2, 3 };
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  std::map<size_t, time_stamp> start_times;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _)).Times(INDICES.size()).WillRepeatedly(Invoke([&start_times](auto& mem, auto index) {
    ASSERT_TRUE(start_times.find(index) == start_times.end());
    start_times[index] = std::chrono::high_resolution_clock::now();
  }));
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _))
    .Times(INDICES.size())
    .WillRepeatedly(Invoke([DELAY_TIME, &start_times](auto& mem, auto index) -> Measurement {
      auto end_time = std::chrono::high_resolution_clock::now();
      EXPECT_TRUE(start_times.find(index) != start_times.end())
            << "Prepare should be called before Sample";
      auto start_time = start_times.at(index);
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
      EXPECT_GE(duration, DELAY_TIME) << "Sampling each round should take more time then the delay";
      return Measurement{};
    }));
  // Code
  ListSampler sampler(INDICES, DELAY_TIME, 0, primitive);
  sampler.Sample(wrapper, measurements);
}

TEST(ListSampler, between_items_delay)
{
  // Consts
  const auto DELAY_TIME = 5000000; // in ns
  const std::vector<size_t> INDICES = { 1, 2, 3 };
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  std::vector<time_stamp> timestamps;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _)).Times(INDICES.size()).WillRepeatedly(Invoke([&timestamps](auto& mem, auto index) {
    timestamps.push_back(std::chrono::high_resolution_clock::now());
  }));
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _)).Times(INDICES.size()).WillRepeatedly(Return(Measurement{}));
  // Code
  ListSampler sampler(INDICES, 0, DELAY_TIME, primitive);
  sampler.Sample(wrapper, measurements);
  // Assertions
  ASSERT_EQ(timestamps.size(), INDICES.size());
  for (size_t i = 1; i < INDICES.size(); i++)
  {
    auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(timestamps[i] - timestamps[i - 1]).count();
    ASSERT_GE(duration, DELAY_TIME) << "Time between items should be at least the given delay";
  }
}

TEST(ListSampler, multiple_samples_parallel)
{
  // Consts
  const int NUM_ITERATIONS = 35;
  const std::vector<size_t> ITEMS = { 1, 2, 3 };
  const auto BASE_TIME = 300;
  const size_t BASE_INDEX = 20;
  // Dependencies
  auto primitive = testutils::Primitive();
  MemoryWrapper wrapper(0);
  std::vector<Measurement> measurements;
  int mock_index = 0;
  // Expectations
  EXPECT_CALL(*primitive, Prepare(Ref(wrapper), _))
    .Times(static_cast<int>(NUM_ITERATIONS * ITEMS.size()))
    .WillRepeatedly(Return());
  EXPECT_CALL(*primitive, Sample(Ref(wrapper), _))
    .Times(static_cast<int>(NUM_ITERATIONS * ITEMS.size()))
    .WillRepeatedly(Invoke([&mock_index](auto& wrapper, auto index) mutable -> Measurement {
      auto res = Measurement{ BASE_INDEX + mock_index, BASE_TIME + mock_index };
      mock_index++;
      return res;
    }));

  // Code
  ListSampler sampler(ITEMS, 0, 0, primitive, ListSamplerMode::Parallel);
  for (auto i = 0; i < NUM_ITERATIONS; i++)
  {
    sampler.Sample(wrapper, measurements);
    ASSERT_EQ(measurements.size(), (i + 1) * ITEMS.size());

    for (size_t j = 0; j < (i + 1) * ITEMS.size(); j++)
    {
      ASSERT_TRUE(measurements[j] == (Measurement{ BASE_INDEX + j, static_cast<int32_t>(BASE_TIME + j) }));
    }
  }
}