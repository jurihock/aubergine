#pragma once

#include <aubergine/Header.h>

#include <aubergine/fx/AudioEffect.h>

class BypassEffect : public AudioEffect
{

public:

  void apply(const uint64_t index, std::span<float> input, std::span<float> output) override
  {
    std::memcpy(output.data(), input.data(), input.size() * sizeof(float));
  }

};
