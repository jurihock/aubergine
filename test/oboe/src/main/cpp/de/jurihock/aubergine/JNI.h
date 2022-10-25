#pragma once

#include <jni.h>

#define JNIVA_COUNT(...) JNIVA_COUNT_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define JNIVA_COUNT_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define JNIVA_IDENTITY(N) N
#define JNIVA_APPLY(MACRO, ...) JNIVA_IDENTITY(MACRO(__VA_ARGS__))

#define JNINAME_DISPATCH(N) JNINAME_JOIN_##N
#define JNINAME_JOIN(PRFX, ...) JNIVA_IDENTITY(JNIVA_APPLY(JNINAME_DISPATCH, JNIVA_COUNT(__VA_ARGS__)))(PRFX, __VA_ARGS__)
#define JNINAME_JOIN_1(PRFX, A) PRFX ## _ ## A
#define JNINAME_JOIN_2(PRFX, A, B) PRFX ## _ ## A ## _ ## B
#define JNINAME_JOIN_3(PRFX, A, B, C) PRFX ## _ ## A ## _ ## B _ ## C

#define JNIARGS_DISPATCH(N) JNIARGS_JOIN_##N
#define JNIARGS_JOIN(PRFX, ...) JNIVA_IDENTITY(JNIVA_APPLY(JNIARGS_DISPATCH, JNIVA_COUNT(__VA_ARGS__)))(PRFX, __VA_ARGS__)
#define JNIARGS_JOIN_1(PRFX, A) PRFX , A
#define JNIARGS_JOIN_2(PRFX, A, B) PRFX , A , B
#define JNIARGS_JOIN_3(PRFX, A, B, C) PRFX , A , B , C

#ifdef __cplusplus
#define JNIEXTERN extern "C"
#else
#define JNIEXTERN
#endif

#define JNIFUNC(...) JNIEXTERN JNIEXPORT __VA_ARGS__
#define JNINAME(...) JNICALL JNINAME_JOIN(Java_de_jurihock_aubergine, __VA_ARGS__)
#define JNIARGS(...) (JNIARGS_JOIN(JNIEnv* jni, __VA_ARGS__))
