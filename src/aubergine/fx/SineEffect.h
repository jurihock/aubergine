#pragma once

#include <aubergine/Header.h>

#include <aubergine/etc/Oscillator.h>
#include <aubergine/fx/AudioEffect.h>

class SineEffect : public AudioEffect
{

public:

  SineEffect(const float amplitude, const float frequency) :
    amplitude(amplitude),
    frequency(frequency)
  {
  }

  void reset(const float samplerate, const size_t buffersize) override
  {
    osc = Oscillator<float>(frequency, samplerate);
  }

  void apply(const uint64_t index, std::span<float> input, std::span<float> output) override
  {
    for (size_t i = 0; i < output.size(); ++i)
    {
      output[i] = amplitude * osc.sin();
    }
  }

private:

  const float amplitude;
  const float frequency;

  Oscillator<float> osc;

};
