#include <aubergine/io/AudioSource.h>

#include <aubergine/Source.h>

AudioSource::AudioSource(const float samplerate, const size_t buffersize, const std::shared_ptr<AudioEffect> effect, const std::shared_ptr<AudioBufferQueue> queue) :
  AudioStream(oboe::Direction::Input, samplerate, buffersize),
  effect(effect),
  queue((queue != nullptr) ? queue : std::make_shared<AudioBufferQueue>())
{
  if (effect)
  {
    onopen([this]()
    {
      this->effect->reset(this->samplerate(), this->buffersize());
    });
  }

  onstart([this]()
  {
    this->index = { 0, 0 };
  });
}

std::shared_ptr<AudioEffect> AudioSource::fx() const
{
  return effect;
}

std::shared_ptr<AudioBufferQueue> AudioSource::fifo() const
{
  return queue;
}

void AudioSource::callback(const std::span<float> samples)
{
  const bool ok = queue->write([&](AudioBuffer& buffer)
  {
    if (effect)
    {
      effect->apply(index.inner, samples, buffer);
    }
    else
    {
      buffer.copyfrom(samples);
    }

    ++index.inner;
  });

  if (!ok)
  {
    // TODO: LOG(WARNING) << $("Audio source fifo overflow! #{0}", index.outer);
  }

  ++index.outer;
}
