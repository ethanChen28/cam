/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef INCLUDE_BASE_H_
#define INCLUDE_BASE_H_

#include <vector>
#include <cstdint>

///> for gcc
#if defined(__GNUC__)

#if defined (DLL_EXPORT)
#define ARCTERN_EXPORT_API __attribute__((visibility("default")))
#else
#define ARCTERN_EXPORT_API
#endif

///> for visual studio
#elif defined(_MSC_VER)

#if !defined(DLL_EXPORT) && defined(_WINDLL)
#define DLL_EXPORT
#endif

#if defined (DLL_EXPORT)
#define ARCTERN_EXPORT_API __declspec(dllexport)
#else
#define ARCTERN_EXPORT_API __declspec(dllimport)
#endif

///> for unknown tool chain
#else
#error "!!unspport this toolchain at now!!"
#endif


///> general global variable decorate macro(for external reference )
#define G_VAR  extern ARCTERN_EXPORT_API

#ifndef uint8
using uint8 = unsigned  char;
#endif

#ifndef int8
using int8 = signed char;
#endif

#ifndef uint16
using uint16 = unsigned short;
#endif

#ifndef int16
using int16 = signed short;
#endif

namespace arctern {

///> \brief single object feature
using ArcternFeature = std::vector<uint8>;

///> \brief error code
typedef enum {
  ERR_SUCCESS = 0,
  ERR_NO_INIT,
  ERR_INIT_PARAM,
  ERR_MODEL_NO_EXIST,
  ERR_GPU_NO_FIND,
  ERR_RUN_PARM,
  ERR_NO_IMAGES,
  ERR_NO_IMAGE_DATA,
  ERR_ARCTERN_IMAGE_MAT,
  ERR_PRE_INIT_PARAM,
  ERR_PRE_RUN_PARAM,
  ERR_PRE_PROCESS,
  ERR_PROCESS,
  ERR_NULL_POINT,
  ERR_MEMDB_FULL,
  ERR_MEMDB_ALLOC,
  ERR_MEMDB_EMPTY,
  ERR_MEMDB_MISMATCH,
  ERR_MEMDB_FEATLENGTH,
  ERR_MEMDB_NOTFOUND,
  ERR_MEMDB_INVALID,
  ERR_INVALID_MODEL,
  ERR_CREATE_ENGINE,
  ERR_UNEXPECT
}ErrorType;

/// \brief image format
typedef enum {
  ///< gray
  ARCTERN_IMAGE_FORMAT_GRAY = 0,
  ///< RGB
  ARCTERN_IMAGE_FORMAT_RGB888 = 1,
  ///< BGR
  ARCTERN_IMAGE_FORMAT_BGR888 = 2,
  ///< NV21
  ARCTERN_IMAGE_FORMAT_NV21 = 3,

} ArcternImageFormat;

/// \brief HiSi image data
typedef struct {
  ///< virtual data
  uint64_t vir_data[3];
  ///< physics data
  uint64_t phy_data[3];
  ///< stride
  uint32_t stride[3];
} ArcternHisiImgData;

/// \brief atlas image data
typedef struct {
  void* rawData ;
  long rawDataLen ;
  int  widthStride ;
  int heightStride ;
}ArcternAtlasImgData;


/// \brief arctern image memory type
typedef enum {
  CPU = 1,
  GPU,
  NPU
} ArcternImageMemType;

/// \brief image
struct ArcternImage {
  ArcternImageFormat format;
  ArcternImageMemType type = CPU;
  int width = 0;
  int height = 0;
  int step = 0;
  union {
    ArcternHisiImgData hsdata;
    ArcternAtlasImgData atlasData;
    ///> managed by user
    ///> if type = cpu,gdata point to cpu memory
    ///> if type = gpu,gdata point to gpu memory
    void *gdata = nullptr;
  } imgdata;
};

/// \brief rectangle
typedef struct {
  float x;
  float y;
  float width;
  float height;
} ArcternRect;

///> \brief point
typedef struct {
  float x;
  float y;
} ArcternPoint;

///> \brief device type, only for tvm
typedef enum {
  ///< CPU
  ARCTERN_CPU = 0,
  ///< Nvidia tensorrt
  ARCTERN_TRT = 1,
  ///<  HUAWEI ATLAS
  ARCTERN_ATLAS = 2,
} ArcternDeviceType;

///> \brief precision
typedef enum {
  ///<  use FP32
  ARCTERN_FP32 = 0,
  ///<  use FP16
  ARCTERN_FP16 = 1,
  ///<  use INT8
  ARCTERN_INT8 = 2,
} ArcternTypeBits;


}  // namespace arctern
#endif  // INCLUDE_BASE_H_
