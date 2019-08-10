#pragma once

#include "../../../lib/sampling/measurement.hpp"
#include "../../../lib/utils/memory_wrapper.hpp"
#include <string>
#include <thread>
#include <utility>
#include <vector>
class Layer
{
public:
  Layer(size_t first_index, size_t second_index, int layer, std::string layer_name) :
    first_index_(first_index),
    second_index_(second_index),
    layer_(layer),
    layer_name_(std::move(layer_name))
  {
  }

  // Sample and return the result
  virtual std::vector<Measurement>& Sample() = 0;
  // Cleanup the current sample
  virtual void Finalize() = 0;

  virtual ~Layer() = default;

  int layer_;
  std::string layer_name_;

  // Sleep the given nanoseconds
  static void Sleep(size_t nanoseconds)
  {
    if (nanoseconds != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }
  }

protected:
  static const auto DELAY = 500000;
  static const auto COUNT = 50;

  size_t first_index_;
  size_t second_index_;
};
