#include <aubergine/io/AudioStream.h>

#include <aubergine/Source.h>

AudioStream::AudioStream(const oboe::Direction direction, const float samplerate, const size_t buffersize) :
  direction(direction)
{
  config.samplerate.target = samplerate;
  config.samplerate.actual = 0;
  config.buffersize.target = buffersize;
  config.buffersize.actual = 0;
  config.buffersize.maximum = 0;
  config.timeout = std::chrono::milliseconds(0);
}

AudioStream::~AudioStream()
{
  close();
}

float AudioStream::samplerate() const
{
  return config.samplerate.actual;
}

size_t AudioStream::buffersize() const
{
  return config.buffersize.actual;
}

size_t AudioStream::maxbuffersize() const
{
  return config.buffersize.maximum;
}

std::chrono::milliseconds AudioStream::timeout() const
{
  return config.timeout;
}

void AudioStream::onopen()
{
  events.open.unsubscribe();
}

void AudioStream::onopen(const std::function<void()> callback)
{
  events.open.subscribe(callback);
}

void AudioStream::onclose()
{
  events.close.unsubscribe();
}

void AudioStream::onclose(const std::function<void()> callback)
{
  events.close.subscribe(callback);
}

void AudioStream::onstart()
{
  events.start.unsubscribe();
}

void AudioStream::onstart(const std::function<void()> callback)
{
  events.start.subscribe(callback);
}

void AudioStream::onstop()
{
  events.stop.unsubscribe();
}

void AudioStream::onstop(const std::function<void()> callback)
{
  events.stop.subscribe(callback);
}

void AudioStream::onxrun()
{
  events.xrun.unsubscribe();
}

void AudioStream::onxrun(const std::function<void(const int32_t count)> callback)
{
  events.xrun.subscribe(callback);
}

void AudioStream::onerror()
{
  events.error.unsubscribe();
}

void AudioStream::onerror(const std::function<bool(const oboe::Result error)> callback)
{
  events.error.subscribe(callback);
}

void AudioStream::open()
{
  if (stream != nullptr)
    return;

  oboe::AudioStreamBuilder builder;

  builder.setDirection(direction);

  builder.setSampleRate(static_cast<int32_t>(config.samplerate.target));
  builder.setFramesPerCallback(static_cast<int32_t>(config.buffersize.target));

  builder.setChannelCount(oboe::ChannelCount::Mono);
  builder.setFormat(oboe::AudioFormat::Float);

  builder.setSharingMode(oboe::SharingMode::Exclusive);
  builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);

  builder.setDataCallback(this);
  builder.setErrorCallback(this);

  const oboe::Result result = builder.openStream(stream);

  if (result != oboe::Result::OK)
  {
    close();

    throw std::runtime_error(
      $("Unable to open audio source stream: {0}",
        oboe::convertToText(result)));
  }

  config.samplerate.actual = stream->getSampleRate();
  config.buffersize.actual = stream->getFramesPerCallback();
  config.buffersize.maximum = stream->getBufferSizeInFrames();

  const int micros = 1e6 * stream->getBufferSizeInFrames() / stream->getSampleRate();
  const int millis = std::max(1, micros / int(1e3));

  config.timeout = std::chrono::milliseconds(millis);

//  LOG(DEBUG) << "~ " << oboe::convertToText(stream->getDirection()) << " ~";
//  LOG(DEBUG) << "DeviceId " << stream->getDeviceId();
//  LOG(DEBUG) << "AudioApi " << oboe::convertToText(stream->getAudioApi());
//  LOG(DEBUG) << "SharingMode " << oboe::convertToText(stream->getSharingMode());
//  LOG(DEBUG) << "PerformanceMode " << oboe::convertToText(stream->getPerformanceMode());
//  LOG(DEBUG) << "SampleRate " << stream->getSampleRate();
//  LOG(DEBUG) << "ChannelCount " << stream->getChannelCount();
//  LOG(DEBUG) << "Format " << oboe::convertToText(stream->getFormat());
//  LOG(DEBUG) << "BufferCapacityInFrames " << stream->getBufferCapacityInFrames();
//  LOG(DEBUG) << "BufferSizeInFrames " << stream->getBufferSizeInFrames();
//  LOG(DEBUG) << "FramesPerBurst " << stream->getFramesPerBurst();
//  LOG(DEBUG) << "FramesPerCallback " << stream->getFramesPerCallback();
//  LOG(DEBUG) << "Timeout " << config.timeout.count() << " ms";

  events.open();
}

void AudioStream::close()
{
  if (stream == nullptr)
    return;

  if (stream->getState() != oboe::StreamState::Closed)
  {
    stream->stop();
    stream->close();
  }

  stream = nullptr;

  events.close();
}

void AudioStream::start()
{
  if (stream == nullptr)
    return;

  events.start();

  xruns = stream->getXRunCount().value();

  stream->start();
}

void AudioStream::stop()
{
  if (stream == nullptr)
    return;

  stream->stop();

  events.stop();
}

oboe::DataCallbackResult AudioStream::onAudioReady(oboe::AudioStream* stream, void* data, int32_t size)
{
  const std::span<float> samples(
    static_cast<float*>(data),
    static_cast<size_t>(size));

  callback(samples);

  const int32_t xruns = stream->getXRunCount().value();

  if (this->xruns != xruns)
  {
    events.xrun(xruns);
  }

  this->xruns = xruns;

  return oboe::DataCallbackResult::Continue;
}

bool AudioStream::onError(oboe::AudioStream* stream, oboe::Result error)
{
  return events.error(error);
}
