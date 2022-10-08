/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLOR_H_
#define SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLOR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸图像颜色运行参数类
class ARCTERN_EXPORT_API FaceImageColorRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceImageColorRunParameter();

  /// \brief 析构函数
  ~FaceImageColorRunParameter();
};

/// \brief 人脸图像颜色结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceImageColorResult : public Result {
 public:
  /// \brief 构造函数
  FaceImageColorResult();

  /// \brief 析构函数
  ~FaceImageColorResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceImageColors_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceImageColors_.resize(n); }

 public:
  std::vector<float> faceImageColors_;  ///< 结果容器, 大于0.5表示为彩色，否则为黑白
};

/// \brief 人脸图像颜色算法类
class ARCTERN_EXPORT_API FaceImageColor : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceImageColor();

  /// \brief 析构函数
  ~FaceImageColor() override;

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
  AlgorithmBase *impl_ = nullptr;    ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLOR_H_
