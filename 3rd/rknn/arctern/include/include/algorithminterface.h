/****************************************************************************
 *  Filename:       algorithminterface.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:    interface for users
 *  author:         juwenqi
 *  Last modified:  2020.02.20
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef INCLUDE_ALGORITHMINTERFACE_H_
#define INCLUDE_ALGORITHMINTERFACE_H_

#include "arctern.h"
#include "result.h"
#include "initparameter.h"
#include "runparameter.h"

namespace arctern {

///> \brief the base class of all of alg class
class ARCTERN_EXPORT_API AlgorithmInterface {
 public:
  AlgorithmInterface() = default;
  virtual ~AlgorithmInterface() = default;

  virtual ErrorType Init(const InitParameter *p) = 0;
  virtual ErrorType Process(const RunParameter *p,Result *r) = 0;
  virtual void Release() = 0;
};

}  // namespace arctern
#endif  // INCLUDE_ALGORITHMINTERFACE_H_
