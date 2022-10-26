#include <aubergine/io/AudioPipeline.h>

#include <aubergine/Source.h>

#include <aubergine/etc/Timer.h>

AudioPipeline::AudioPipeline(const std::shared_ptr<AudioSource> source, const std::shared_ptr<AudioSink> sink, const std::shared_ptr<AudioEffect> effect) :
  source(source), sink(sink), effect(effect), doloop(false)
{
}

AudioPipeline::~AudioPipeline()
{
  close();
}

void AudioPipeline::onerror()
{
  events.error.reset();
}

void AudioPipeline::onerror(std::function<void()> callback)
{
  events.error.set(callback);
}

void AudioPipeline::open()
{
  source->open();
  sink->open();

  if (source->samplerate() != sink->samplerate())
  {
    throw std::runtime_error(
      $("Unequal audio stream sample rate: {0} (source), {1} (sink)!",
        source->samplerate(), sink->samplerate()));
  }

  if (source->samplerate() == 0 || sink->samplerate() == 0)
  {
    throw std::runtime_error(
      $("Invalid audio stream sample rate: {0} (source), {1} (sink)!",
        source->samplerate(), sink->samplerate()));
  }

  if (source->buffersize() != sink->buffersize())
  {
    throw std::runtime_error(
      $("Unequal audio stream buffer size: {0} (source), {1} (sink)!",
        source->buffersize(), sink->buffersize()));
  }

  if (source->buffersize() == 0 || sink->buffersize() == 0)
  {
    throw std::runtime_error(
      $("Invalid audio stream buffer size: {0} (source), {1} (sink)!",
        source->buffersize(), sink->buffersize()));
  }

  if (source->maxbuffersize() == 0 || sink->maxbuffersize() == 0)
  {
    throw std::runtime_error(
      $("Invalid audio stream max. buffer size: {0} (source), {1} (sink)!",
        source->maxbuffersize(), sink->maxbuffersize()));
  }

  if (effect)
  {
    effect->reset(source->samplerate(), source->buffersize());
  }

  const size_t fifosize = 10 *
    std::max(source->maxbuffersize(), sink->maxbuffersize()) /
    std::min(source->buffersize(), sink->buffersize());

  source->fifo()->resize(fifosize, source->buffersize());
  sink->fifo()->resize(fifosize, sink->buffersize());

  source->onxrun([&](const int32_t count)
  {
    onxrun(oboe::Direction::Input, count);
  });

  sink->onxrun([&](const int32_t count)
  {
    onxrun(oboe::Direction::Output, count);
  });

  source->onerror([&](const oboe::Result error)
  {
    return onerror(oboe::Direction::Input, error);
  });

  sink->onerror([&](const oboe::Result error)
  {
    return onerror(oboe::Direction::Output, error);
  });
}

void AudioPipeline::close()
{
  stop();

  source->onxrun();
  sink->onxrun();

  source->onerror();
  sink->onerror();

  source->close();
  sink->close();
}

void AudioPipeline::start()
{
  doloop = true;

  source->start();
  sink->start();

  loopthread = std::make_shared<std::thread>([&]()
  {
    loop();
  });
}

void AudioPipeline::stop()
{
  doloop = false;

  if (loopthread != nullptr)
  {
    if (loopthread->joinable())
    {
      loopthread->join();
    }

    loopthread = nullptr;
  }

  sink->stop();
  source->stop();

  sink->fifo()->flush();
  source->fifo()->flush();
}

void AudioPipeline::loop()
{
  struct timers_t
  {
    Timer<std::chrono::milliseconds> outer;
    Timer<std::chrono::milliseconds> inner;
  }
  timers;

  const auto dowork = [this](uint64_t& index, timers_t& timers, const std::chrono::milliseconds timeout)
  {
    const bool ok = source->fifo()->read(timeout, [&](AudioBuffer& input)
    {
      timers.outer.toc();
      timers.outer.tic();

      const bool ok = sink->fifo()->write([&](AudioBuffer& output)
      {
        timers.inner.tic();

        if (effect)
        {
          effect->apply(index, input, output);
        }
        else
        {
          input.copyto(output);
        }

        timers.inner.toc();

        ++index;
      });

      if (!ok)
      {
        // TODO: LOG(WARNING) << $("Audio pipe fifo overflow!");
      }
    });

    if (!ok)
    {
      // TODO: LOG(WARNING) << $("Audio pipe fifo underflow!");
    }
  };

  auto millis = [](const std::chrono::steady_clock::duration& duration)
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  };

  auto now = []()
  {
    return std::chrono::steady_clock::now();
  };

  auto index = uint64_t(0);
  auto timestamp = now();

  timers.outer.tic();

  if (doloop)
  {
    dowork(index, timers, source->timeout() * 2);
  }

  while (doloop)
  {
    dowork(index, timers, source->timeout());

    if (millis(now() - timestamp) > 5000)
    {
      // TODO

//      LOG(INFO)
//        << "Timing: "
//        << "inner " << timers.inner.str() << " / "
//        << "outer " << timers.outer.str();

      timers.outer.cls();
      timers.inner.cls();

      timestamp = now();
    }
  }
}

void AudioPipeline::onxrun(const oboe::Direction direction, const int32_t count)
{
  switch (direction)
  {
    case oboe::Direction::Input:
      // TODO: LOG(WARNING) << $("Audio source {0} overruns occured!", count);
      break;
    case oboe::Direction::Output:
      // TODO: LOG(WARNING) << $("Audio sink {0} underruns occured!", count);
      break;
  }
}

bool AudioPipeline::onerror(const oboe::Direction direction, const oboe::Result error)
{
  std::unique_lock lock(onerrormutex);

  // TODO: LOG(WARNING) << $("Aborting audio pipeline due to {0} error {1}!", oboe::convertToText(direction), oboe::convertToText(error));

  close();

  events.error();

  return true;
}
