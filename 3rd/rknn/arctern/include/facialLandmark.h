/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARK_H_
#define SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARK_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸关键点运行参数类
class ARCTERN_EXPORT_API FacialLandmarkRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FacialLandmarkRunParameter();

  /// \brief 析构函数
  ~FacialLandmarkRunParameter() override;
};

/// \brief single face landmarks num = 68
using SingleFacialLandmarks = std::vector<ArcternPoint>;

/// \brief 人脸关键点结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FacialLandmarkResult : public Result {
 public:
  /// \brief 构造函数
  FacialLandmarkResult();

  /// \brief 析构函数
  ~FacialLandmarkResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { landmarks_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { landmarks_.resize(n); }
 public:
  std::vector<SingleFacialLandmarks> landmarks_;  ///< 结果容器
};

/// \brief 人脸关键点算法类
class ARCTERN_EXPORT_API FacialLandmark : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FacialLandmark();

  /// \brief 析构函数
  ~FacialLandmark() override;

  /// \brief 算法初始化
  /// \param p - 初始化参数
  /// \return 错误码
  ErrorType Init(const InitParameter *p) override;

  /// \brief 释放资源
  void Release() override;

  /// \brief 执行算法
  /// \param p - 运行参数
  /// \return 算法结果
  ErrorType Process(const RunParameter *p,Result *r) override;



 private:
  AlgorithmBase *impl_ = nullptr;   ///< 算法实现类
};
}  /// namespace arctern
#endif  // SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARK_H_
