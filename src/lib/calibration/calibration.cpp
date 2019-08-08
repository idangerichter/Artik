#include "calibration.hpp"
#include "gap_score_provider.hpp"
#include <algorithm>
#include <functional>

constexpr double THRESHOLD_ACCESSED_MULTIPLIER = 0.9;
constexpr double THRESHOLD_FLUSHED_MULTIPLIER = 0.9;

namespace Calibration
{
std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement>& accessed_measurements,
                                         std::vector<Measurement>& flushed_measurements,
                                         AttackType type)
{
  std::function<bool(int, int)> accessed_comparator = std::greater<>();
  std::function<bool(int, int)> flushed_comparator = std::less<>();

  if (type != AttackType::FlushReload)
  {
    accessed_comparator = std::less<>();
    flushed_comparator = std::greater<>();
  }

  auto accessed_index = static_cast<size_t>(accessed_measurements.size() * THRESHOLD_ACCESSED_MULTIPLIER);
  auto flushed_index = static_cast<size_t>(flushed_measurements.size() * THRESHOLD_FLUSHED_MULTIPLIER);

  auto typeCoerced = type == AttackType ::FlushReload;
  std::nth_element(accessed_measurements.begin(), accessed_measurements.begin() + accessed_index,
                   accessed_measurements.end(), [typeCoerced](Measurement& lhs, Measurement& rhs) -> bool {
                     return (lhs.time > rhs.time) ^ typeCoerced;
                   });
  std::nth_element(flushed_measurements.begin(), flushed_measurements.begin() + flushed_index,
                   flushed_measurements.end(), [typeCoerced](Measurement& lhs, Measurement& rhs) -> bool {
                     return (lhs.time < rhs.time) ^ typeCoerced;
                   });


  auto accessed_threshold = accessed_measurements[accessed_index].time;
  auto flushed_threshold = flushed_measurements[flushed_index].time;

  return std::make_unique<GapScoreProvider>(type, accessed_threshold, flushed_threshold);
}
} // namespace Calibration
