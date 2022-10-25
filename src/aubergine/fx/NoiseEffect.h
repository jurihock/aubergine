#pragma once

#include <aubergine/Header.h>

#include <aubergine/etc/Noise.h>
#include <aubergine/fx/AudioEffect.h>

class NoiseEffect : public AudioEffect
{

public:

  NoiseEffect(const float amplitude) :
    amplitude(amplitude)
  {
  }

  void apply(const uint64_t index, std::span<float> input, std::span<float> output) override
  {
    for (size_t i = 0; i < output.size(); ++i)
    {
      output[i] = amplitude * noise();
    }
  }

private:

  const float amplitude;

  Noise<float> noise;

};
