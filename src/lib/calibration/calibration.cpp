#include "calibration.hpp"
#include "gap_score_provider.hpp"
#include "simple_score_provider.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
constexpr double THRESHOLD_FLUSHED_MULTIPLIER = 0.9;

namespace Calibration
{
std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement>& measurements_after_flush,
                                         AttackType type)
{
  std::function<bool(int, int)> comparator = std::greater<>();

  if (type != AttackType::FlushReload)
  {
    comparator = std::less<>();
  }

  auto index = static_cast<size_t>(measurements_after_flush.size() * THRESHOLD_FLUSHED_MULTIPLIER);

  std::nth_element(measurements_after_flush.begin(),
                   measurements_after_flush.begin() + index,
                   measurements_after_flush.end(),
                   [comparator](Measurement& lhs, Measurement& rhs) -> bool {
                     return comparator(lhs.time, rhs.time);
                   });

  auto threshold = measurements_after_flush[index].time;

  return std::make_unique<SimpleScoreProvider>(type, threshold);
}
} // namespace Calibration
