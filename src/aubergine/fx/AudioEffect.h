#pragma once

#include <aubergine/Header.h>

class AudioEffect
{

public:

  virtual ~AudioEffect() {};

  virtual void reset(const float samplerate, const size_t buffersize) {};
  virtual void apply(const uint64_t index, std::span<float> input, std::span<float> output) = 0;

};
