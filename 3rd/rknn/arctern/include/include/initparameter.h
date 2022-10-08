/****************************************************************************
 *  Filename:       initparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef INCLUDE_INITPARAMETER_H_
#define INCLUDE_INITPARAMETER_H_

#include <string>
#include "arctern.h"

namespace arctern {

typedef struct {
  ///< batch size
  int batchNum_ = 1;
  ///< model path
  std::string model_path;
  ///< device type
  arctern::ArcternDeviceType deviceType_ = ARCTERN_CPU;
  ///< models precision
  arctern::ArcternTypeBits typeBits_ = ARCTERN_FP32;
  ///< gpu id (-1, CPU)
  int gpuId_ = 0;
  ///< thresh
  float thresh_ = 0;
} InitParameter;

}  // namespace arctern
#endif  // INCLUDE_INITPARAMETER_H_
