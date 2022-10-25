#pragma once

#include <aubergine/Header.h>

#include <aubergine/fx/AudioEffect.h>

class NullEffect : public AudioEffect
{

public:

  void apply(const uint64_t index, std::span<float> input, std::span<float> output) override
  {
    std::memset(output.data(), 0, output.size() * sizeof(float));
  }

};
