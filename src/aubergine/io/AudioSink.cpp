#include <aubergine/io/AudioSink.h>

#include <aubergine/Source.h>

AudioSink::AudioSink(const float samplerate, const size_t buffersize, const std::shared_ptr<AudioEffect> effect, const std::shared_ptr<AudioBufferQueue> queue) :
  AudioStream(oboe::Direction::Output, samplerate, buffersize),
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
    this->underflows = { false, 0 };
  });
}

std::shared_ptr<AudioEffect> AudioSink::fx() const
{
  return effect;
}

std::shared_ptr<AudioBufferQueue> AudioSink::fifo() const
{
  return queue;
}

void AudioSink::callback(const std::span<float> samples)
{
  const bool ok = queue->read([&](AudioBuffer& buffer)
  {
    if (effect)
    {
      effect->apply(index.inner, buffer, samples);
    }
    else
    {
      buffer.copyto(samples);
    }

    ++index.inner;
  });

  if (ok)
  {
    if (underflows.accumulate)
    {
      // TODO: LOG(INFO) << $("Audio sink {0} passed callbacks", underflows.count);

      underflows = { false, 0 };
    }
  }
  else
  {
    if (underflows.accumulate)
    {
      ++underflows.count;
    }
    else if (!index.outer)
    {
      underflows = { true, 1 };
    }
    else
    {
      // TODO: LOG(WARNING) << $("Audio sink fifo underflow! #{0}", index.outer);
    }
  }

  ++index.outer;
}
