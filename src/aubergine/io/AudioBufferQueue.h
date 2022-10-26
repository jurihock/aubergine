#pragma once

#include <aubergine/Header.h>

#include <aubergine/etc/FIFO.h>
#include <aubergine/io/AudioBuffer.h>

class AudioBufferQueue final : public FIFO<AudioBuffer>
{

public:

  void resize(const size_t queuesize, const size_t buffersize);

};
