#include "calibration.hpp"
#include "score_provider.hpp"

class GapScoreProvider : public ScoreProvider
{
public:
  explicit GapScoreProvider(AttackType type, double low_threshold, double high_threshold);
  double Normalize(Measurement measurement) const override;

private:
  const AttackType attack_type_;
  const double low_threshold_;
  const double high_threshold_;
};
