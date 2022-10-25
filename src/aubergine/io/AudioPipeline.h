#pragma once

#include <aubergine/Header.h>

#include <aubergine/fx/AudioEffect.h>
#include <aubergine/io/AudioSink.h>
#include <aubergine/io/AudioSource.h>

class AudioPipeline
{

public:

  AudioPipeline(const std::shared_ptr<AudioSource> source, const std::shared_ptr<AudioSink> sink, const std::shared_ptr<AudioEffect> effect = nullptr);
  ~AudioPipeline();

  void onerror();
  void onerror(std::function<void()> callback);

  void open();
  void close();

  void start();
  void stop();

private:

  struct
  {
    Event<void()> error = [](){};
  }
  events;

  const std::shared_ptr<AudioSource> source;
  const std::shared_ptr<AudioSink> sink;
  const std::shared_ptr<AudioEffect> effect;

  std::shared_ptr<std::thread> loopthread;
  bool doloop;
  void loop();

  void onxrun(const oboe::Direction direction, const int32_t count);

  std::mutex onerrormutex;
  bool onerror(const oboe::Direction direction, const oboe::Result error);

};
