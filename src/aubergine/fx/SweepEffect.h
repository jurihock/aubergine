#pragma once

#include <aubergine/Header.h>

#include <aubergine/etc/Wobbulator.h>
#include <aubergine/fx/AudioEffect.h>

class SweepEffect : public AudioEffect
{

public:

  SweepEffect(float amplitude, std::pair<float, float> frequencies, float period) :
    amplitude(amplitude),
    frequencies(frequencies),
    period(period)
  {
  }

  void reset(const float samplerate, const size_t buffersize) override
  {
    osc = Wobbulator<float>(frequencies, period, samplerate);
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
  std::pair<float, float> frequencies;
  float period;

  Wobbulator<float> osc;

};
