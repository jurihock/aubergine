#pragma once

#include <aubergine/Header.h>

class AudioBuffer
{

public:

  AudioBuffer(const size_t capacity);

  size_t capacity() const;

  void copyfrom(const std::span<float> samples);
  void copyfrom(const AudioBuffer& other);

  void copyto(const std::span<float> samples) const;
  void copyto(AudioBuffer& other) const;

  operator std::span<float>() const;

private:

  struct
  {
    std::vector<float> memory;
    size_t capacity;
    size_t size;
    float* data;
  }
  buffer;

};
