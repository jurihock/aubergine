#pragma once

#include <de/jurihock/aubergine/JNI.h>

JNIFUNC(jstring) JNINAME(Test, hello) JNIARGS(jobject);

JNIFUNC(jlong) JNINAME(Test, start) JNIARGS(jobject);
JNIFUNC(jlong) JNINAME(Test, stop) JNIARGS(jobject, jlong);
