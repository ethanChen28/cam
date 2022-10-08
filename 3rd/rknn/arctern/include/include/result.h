/****************************************************************************
 *  Filename:       result.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef INCLUDE_RESULT_H_
#define INCLUDE_RESULT_H_
#include "arctern.h"

namespace arctern {

class ARCTERN_EXPORT_API Result {
 public:
  Result() = default;
  virtual ~Result() = default;
  virtual void Release() = 0;
  virtual void resize(int n) = 0;
};

}  // namespace arctern
#endif  // INCLUDE_RESULT_H_
