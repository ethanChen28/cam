/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEGLUON_FACEGLUON_H_
#define SRC_ALGORITHMS_FACEGLUON_FACEGLUON_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

using GluonLandmarks = std::vector<ArcternPoint>;

/// \brief 人脸特征提取运行参数类
class ARCTERN_EXPORT_API FaceGluonRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceGluonRunParameter();

  /// \brief 析构函数
  ~FaceGluonRunParameter() override;

 public:
  std::vector<bool> faceMaskV_;     ///< 人脸口罩信息，3.9.4模型需要此信息，用来提高算法精度, 如果此参数为空，则表示全不佩戴口罩
  std::vector<GluonLandmarks> faceLandmarksV_;   ///< 人脸关键点，部分识别算法需要人脸关键点。是否需要请查看运行log
};

/// \brief 人脸特征提取结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceGluonResult : public Result {
 public:
  /// \brief 构造函数
  FaceGluonResult();

  /// \brief 析构函数
  ~FaceGluonResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { features_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { features_.resize(n); }
 public:
  std::vector<ArcternFeature> features_;  ///< 结果容器
};

/// \brief 人脸特征提取算法类
class ARCTERN_EXPORT_API FaceGluon : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceGluon();

  /// \brief 析构函数
  ~FaceGluon() override;

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
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEGLUON_FACEGLUON_H_
