#pragma once

#include <aubergine/Header.h>

#include <oboe/Oboe.h>

class AudioStream : private oboe::AudioStreamDataCallback, private oboe::AudioStreamErrorCallback
{

public:

  AudioStream(const oboe::Direction direction, const float samplerate = oboe::Unspecified, const size_t buffersize = oboe::Unspecified);
  ~AudioStream();

  float samplerate() const;
  size_t buffersize() const;
  size_t maxbuffersize() const;
  std::chrono::milliseconds timeout() const;

  void onopen();
  void onopen(const std::function<void()> callback);

  void onclose();
  void onclose(const std::function<void()> callback);

  void onstart();
  void onstart(const std::function<void()> callback);

  void onstop();
  void onstop(const std::function<void()> callback);

  void onxrun();
  void onxrun(const std::function<void(const int32_t count)> callback);

  void onerror();
  void onerror(const std::function<bool(const oboe::Result error)> callback);

  void open();
  void close();

  void start();
  void stop();

protected:

  virtual void callback(const std::span<float> samples) = 0;

private:

  const oboe::Direction direction;

  struct
  {
    struct
    {
      float target;
      float actual;
    }
    samplerate;

    struct
    {
      size_t target;
      size_t actual;
      size_t maximum;
    }
    buffersize;

    std::chrono::milliseconds timeout;
  }
  config;

  struct
  {
    Event<void()> open = [](){};
    Event<void()> close = [](){};
    Event<void()> start = [](){};
    Event<void()> stop = [](){};
    Event<void(int32_t)> xrun = [](int32_t){};
    Event<bool(oboe::Result)> error = [](oboe::Result){ return false; };
  }
  events;

  std::shared_ptr<oboe::AudioStream> stream;

  int32_t xruns;

  oboe::DataCallbackResult onAudioReady(oboe::AudioStream* stream, void* data, int32_t size) override;
  bool onError(oboe::AudioStream* stream, oboe::Result error) override;

};
