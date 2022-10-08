/****************************************************************************
 *  Filename:       runparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef INCLUDE_RUNPARAMETER_H_
#define INCLUDE_RUNPARAMETER_H_
#include <vector>
#include <iostream>
#include "arctern.h"

using namespace std;

namespace arctern {

/// \brief runtime parameter
class ARCTERN_EXPORT_API RunParameter {
 public:
  RunParameter() = default;
  virtual ~RunParameter() = default;
  ///> check whether rectsV_ size equal imageV_ size
  virtual ErrorType CheckSize() const;
  ///> picture to be inferred
  std::vector<ArcternImage> imageV_;
  ///> face,person,vehicle rectangle or other rectangle
  ///> rectsV_ size must equal imageV_ size
  ///> Attention: detect alg not need to init
  std::vector<ArcternRect> rectsV_;

};
}  // namespace arctern
#endif  // INCLUDE_RUNPARAMETER_H_
