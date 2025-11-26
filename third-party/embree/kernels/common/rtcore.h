// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../../include/embree4/rtcore.h"
RTC_NAMESPACE_USE

namespace embree
{  
  /*! decoding of intersection flags */
  __forceinline bool isCoherent  (RTCRayQueryFlags flags) { return (flags & RTC_RAY_QUERY_FLAG_COHERENT) == RTC_RAY_QUERY_FLAG_COHERENT; }
  __forceinline bool isIncoherent(RTCRayQueryFlags flags) { return (flags & RTC_RAY_QUERY_FLAG_COHERENT) == RTC_RAY_QUERY_FLAG_INCOHERENT; }

/*! Macros used in the rtcore API implementation */
// Rebel changes start.
#  define RTC_CATCH_BEGIN
#  define RTC_CATCH_END(device)
#  define RTC_CATCH_END2(scene)
#  define RTC_CATCH_END2_FALSE(scene) return false;
// Rebel changes end.
  
#define RTC_VERIFY_HANDLE(handle)                               \
  if (handle == nullptr) {                                         \
    throw_RTCError(RTC_ERROR_INVALID_ARGUMENT,"invalid argument"); \
  }

#define RTC_VERIFY_GEOMID(id)                                   \
  if (id == RTC_INVALID_GEOMETRY_ID) {                             \
    throw_RTCError(RTC_ERROR_INVALID_ARGUMENT,"invalid argument"); \
  }

#define RTC_VERIFY_UPPER(id,upper)                              \
  if (id > upper) {                                                \
    throw_RTCError(RTC_ERROR_INVALID_ARGUMENT,"invalid argument"); \
  }

#define RTC_VERIFY_RANGE(id,lower,upper)	\
  if (id < lower || id > upper)						  \
    throw_RTCError(RTC_ERROR_INVALID_OPERATION,"argument out of bounds");
  
#if 0 // enable to debug print all API calls
#define RTC_TRACE(x) std::cout << #x << std::endl;
#else
#define RTC_TRACE(x) 
#endif

// Rebel changes start.
#define throw_RTCError(error,str) abort();
// Rebel changes end.

#define RTC_BUILD_ARGUMENTS_HAS(settings,member) \
  (settings.byteSize > (offsetof(RTCBuildArguments,member)+sizeof(settings.member)))

  
  inline void storeTransform(const AffineSpace3fa& space, RTCFormat format, float* xfm)
  {
    switch (format)
    {
    case RTC_FORMAT_FLOAT3X4_ROW_MAJOR:
      xfm[ 0] = space.l.vx.x;  xfm[ 1] = space.l.vy.x;  xfm[ 2] = space.l.vz.x;  xfm[ 3] = space.p.x;
      xfm[ 4] = space.l.vx.y;  xfm[ 5] = space.l.vy.y;  xfm[ 6] = space.l.vz.y;  xfm[ 7] = space.p.y;
      xfm[ 8] = space.l.vx.z;  xfm[ 9] = space.l.vy.z;  xfm[10] = space.l.vz.z;  xfm[11] = space.p.z;
      break;

    case RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR:
      xfm[ 0] = space.l.vx.x;  xfm[ 1] = space.l.vx.y;  xfm[ 2] = space.l.vx.z;
      xfm[ 3] = space.l.vy.x;  xfm[ 4] = space.l.vy.y;  xfm[ 5] = space.l.vy.z;
      xfm[ 6] = space.l.vz.x;  xfm[ 7] = space.l.vz.y;  xfm[ 8] = space.l.vz.z;
      xfm[ 9] = space.p.x;     xfm[10] = space.p.y;     xfm[11] = space.p.z;
      break;

    case RTC_FORMAT_FLOAT4X4_COLUMN_MAJOR:
      xfm[ 0] = space.l.vx.x;  xfm[ 1] = space.l.vx.y;  xfm[ 2] = space.l.vx.z;  xfm[ 3] = 0.f;
      xfm[ 4] = space.l.vy.x;  xfm[ 5] = space.l.vy.y;  xfm[ 6] = space.l.vy.z;  xfm[ 7] = 0.f;
      xfm[ 8] = space.l.vz.x;  xfm[ 9] = space.l.vz.y;  xfm[10] = space.l.vz.z;  xfm[11] = 0.f;
      xfm[12] = space.p.x;     xfm[13] = space.p.y;     xfm[14] = space.p.z;     xfm[15] = 1.f;
      break;

    default:
#if !defined(__SYCL_DEVICE_ONLY__)
      throw_RTCError(RTC_ERROR_INVALID_OPERATION, "invalid matrix format");
#endif
      break;
    }
  }
}
