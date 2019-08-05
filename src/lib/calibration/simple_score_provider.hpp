#include "calibration.hpp"
#include "score_provider.hpp"

class SimpleScoreProvider : public ScoreProvider
{
public:
  explicit SimpleScoreProvider(AttackType type, double threshold);
  double Normalize(Measurement measurement) const override;

private:
  const AttackType attack_type_;
  const double threshold_;
};
