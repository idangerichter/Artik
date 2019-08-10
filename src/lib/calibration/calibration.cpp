#include "calibration.hpp"
#include "gap_score_provider.hpp"
#include "simple_score_provider.hpp"
#include <algorithm>
#include <functional>

constexpr double THRESHOLD_FLUSHED_MULTIPLIER = 0.9;

namespace Calibration
{
std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement>& measurements_after_flush,
                                         AttackType type)
{
  std::function<bool(int, int)> comparator = std::less<>();

  if (type != AttackType::FlushReload)
  {
    comparator = std::greater<>();
  }

  auto index = static_cast<size_t>(measurements_after_flush.size() * THRESHOLD_FLUSHED_MULTIPLIER);

  auto typeCoerced = type == AttackType ::FlushReload;
  std::nth_element(measurements_after_flush.begin(),
                   measurements_after_flush.begin() + index,
                   measurements_after_flush.end(),
                   [typeCoerced](Measurement& lhs, Measurement& rhs) -> bool {
                     return (lhs.time < rhs.time) ^ typeCoerced;
                   });


  auto threshold = measurements_after_flush[index].time;

  return std::make_unique<SimpleScoreProvider>(type, threshold);
}
} // namespace Calibration
