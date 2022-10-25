#include <aubergine/io/AudioBufferQueue.h>

#include <aubergine/Source.h>

void AudioBufferQueue::resize(const size_t queuesize, const size_t buffersize)
{
  FIFO<AudioBuffer>::resize(
    queuesize, [buffersize]() { return new AudioBuffer(buffersize); },
    [](AudioBuffer* buffer) { delete buffer; });
}
