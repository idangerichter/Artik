#pragma once

#include "../../../lib/attack/attack_manager.hpp"
#include "layer.hpp"
class LayerAttackManagerWithoutAverage : public Layer
{
public:
  LayerAttackManagerWithoutAverage(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  std::vector<AttackResult> temp_attack_results_;
  AttackManager attack_manager_;

};
