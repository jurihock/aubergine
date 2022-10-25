#pragma once

#include <aubergine/Header.h>

#include <aubergine/fx/AudioEffect.h>
#include <aubergine/io/AudioBufferQueue.h>
#include <aubergine/io/AudioStream.h>

class AudioSource : public AudioStream
{

public:

  AudioSource(const float samplerate = oboe::Unspecified, const size_t buffersize = oboe::Unspecified, const std::shared_ptr<AudioEffect> effect = nullptr, const std::shared_ptr<AudioBufferQueue> queue = nullptr);

  std::shared_ptr<AudioEffect> fx() const;
  std::shared_ptr<AudioBufferQueue> fifo() const;

protected:

  void callback(const std::span<float> samples) override;

private:

  const std::shared_ptr<AudioEffect> effect;
  const std::shared_ptr<AudioBufferQueue> queue;

  struct
  {
    uint64_t inner;
    uint64_t outer;
  }
  index;

};
