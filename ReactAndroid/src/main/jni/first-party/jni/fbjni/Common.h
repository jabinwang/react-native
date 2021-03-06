/*
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

/** @file Common.h
 *
 * Defining the stuff that don't deserve headers of their own...
 */

#pragma once

#include <jni.h>

#include "../Environment.h"
#include "../ALog.h"

/// @cond INTERNAL

namespace facebook {
namespace jni {

/**
 * This needs to be called at library load time, typically in your JNI_OnLoad method.
 *
 * The intended use is to return the result of initialize() directly
 * from JNI_OnLoad and to do nothing else there. Library specific
 * initialization code should go in the function passed to initialize
 * (which can be, and probably should be, a C++ lambda). This approach
 * provides correct error handling and translation errors during
 * initialization into Java exceptions when appropriate.
 *
 * Failure to call this will cause your code to crash in a remarkably
 * unhelpful way (typically a segfault) while trying to handle an exception
 * which occurs later.
 */
jint initialize(JavaVM*, void(*)()) noexcept;

namespace internal {

/**
 * Retrieve a pointer the JNI environment of the current thread.
 *
 * @pre The current thread must be attached to the VM
 */
inline JNIEnv* getEnv() noexcept {
  // TODO(T6594868) Benchmark against raw JNI access
  return Environment::current();
}

// Define to get extremely verbose logging of references and to enable reference stats
#if defined(__ANDROID__) && defined(FBJNI_DEBUG_REFS)
template<typename... Args>
inline void dbglog(Args... args) noexcept {
  facebook::alog::logv("fbjni_ref", args...);
}
#else
template<typename... Args>
inline void dbglog(Args...) noexcept {}
#endif

}}}

/// @endcond
