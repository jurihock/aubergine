#include <aubergine/io/AudioBuffer.h>

#include <aubergine/Source.h>

AudioBuffer::AudioBuffer(const size_t capacity)
{
  buffer.memory.resize(capacity);
  buffer.capacity = buffer.memory.capacity();
  buffer.size = buffer.memory.capacity();
  buffer.data = buffer.memory.data();
}

size_t AudioBuffer::capacity() const
{
  return buffer.capacity;
}

void AudioBuffer::copyfrom(const std::span<float> samples)
{
  if (buffer.size != samples.size())
  {
    LOG(WARNING) << $("Unequal buffer size: {0} (this), {1} (other)",
                      buffer.size, samples.size());
  }

  const size_t size = std::min(buffer.size, samples.size());

  std::memcpy(buffer.data, samples.data(), size * sizeof(float));
}

void AudioBuffer::copyfrom(const AudioBuffer& other)
{
  if (buffer.size != other.buffer.size)
  {
    LOG(WARNING) << $("Unequal buffer size: {0} (this), {1} (other)",
                      buffer.size, other.buffer.size);
  }

  const size_t size = std::min(buffer.size, other.buffer.size);

  std::memcpy(buffer.data, other.buffer.data, size * sizeof(float));
}

void AudioBuffer::copyto(const std::span<float> samples) const
{
  if (buffer.size != samples.size())
  {
    LOG(WARNING) << $("Unequal buffer size: {0} (this), {1} (other)",
                      buffer.size, samples.size());
  }

  const size_t size = std::min(buffer.size, samples.size());

  std::memcpy(samples.data(), buffer.data, size * sizeof(float));
}

void AudioBuffer::copyto(AudioBuffer& other) const
{
  if (buffer.size != other.buffer.size)
  {
    LOG(WARNING) << $("Unequal buffer size: {0} (this), {1} (other)",
                      buffer.size, other.buffer.size);
  }

  const size_t size = std::min(buffer.size, other.buffer.size);

  std::memcpy(other.buffer.data, buffer.data, size * sizeof(float));
}

AudioBuffer::operator std::span<float>() const
{
  return std::span<float>(buffer.data, buffer.size);
}
