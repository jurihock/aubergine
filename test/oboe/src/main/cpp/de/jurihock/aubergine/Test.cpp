#include <de/jurihock/aubergine/Test.h>

#include <aubergine/fx/BypassEffect.h>
#include <aubergine/fx/NoiseEffect.h>
#include <aubergine/fx/NullEffect.h>
#include <aubergine/fx/SineEffect.h>
#include <aubergine/fx/SweepEffect.h>

#include <aubergine/io/AudioPipeline.h>
#include <aubergine/io/AudioSink.h>
#include <aubergine/io/AudioSource.h>

JNIFUNC(jstring) JNINAME(Test, hello) JNIARGS(jobject)
{
  const std::string hello = "Hello World!";
  return jni->NewStringUTF(hello.c_str());
}

JNIFUNC(jlong) JNINAME(Test, start) JNIARGS(jobject)
{
  auto bypass = std::make_shared<BypassEffect>();
  auto noise = std::make_shared<NoiseEffect>(1.f);
  auto null = std::make_shared<NullEffect>();
  auto sine = std::make_shared<SineEffect>(1.f, 440.f);
  auto sweep = std::make_shared<SweepEffect>(1.f, std::make_pair(500.f, 1500.f), 3.f);

  auto source = std::make_shared<AudioSource>(0, 512, sine);
  auto sink = std::make_shared<AudioSink>(0, 512);

  auto pipe = new AudioPipeline(source, sink, bypass);

  pipe->open();
  pipe->start();

  return reinterpret_cast<jlong>(pipe);
}

JNIFUNC(jlong) JNINAME(Test, stop) JNIARGS(jobject, jlong pointer)
{
  auto pipe = reinterpret_cast<AudioPipeline*>(pointer);

  pipe->stop();
  pipe->close();

  delete pipe;

  return 0;
}
