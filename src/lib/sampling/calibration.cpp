#include "calibration.hpp"
#include <iostream>

const double THRESHOLD = 220;


class SimpleScoreProvider : public ScoreProvider
{
    public:
    explicit SimpleScoreProvider(AttackType attackType) : attackType(attackType)
    {
    }

    public:
    double normalize(Measurement measurement) const override
    {
        switch (attackType)
        {
        case AttackType::FlushReload:
            return measurement.time >= THRESHOLD ? 0.0 : 1.0;
        case AttackType::PrimeProbe:
            return measurement.time >= THRESHOLD ? 1.0 : 0.0;
        }
    }

    private:
    AttackType attackType;
};

std::unique_ptr<ScoreProvider> Calibration::Calibrate(std::vector<Measurement> measurments, AttackType type)
{
   return std::make_unique<SimpleScoreProvider>(type);
}
