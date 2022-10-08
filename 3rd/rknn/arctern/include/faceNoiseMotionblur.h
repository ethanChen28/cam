/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLUR_H_
#define SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLUR_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸运动质量运行参数类
class ARCTERN_EXPORT_API FaceNoiseMotionblurRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceNoiseMotionblurRunParameter();

  /// \brief 析构函数
  ~FaceNoiseMotionblurRunParameter();
};

/// \brief  人脸运动质量类型
typedef enum {
  MOTIONBLUR = 0,  ///< 运动模糊
  NOISY = 1,      ///< 有噪点
  NORMAL = 2,     ///< 正常
} NoiseType;

/// \brief 单人脸运动质量信息
typedef struct {
  NoiseType type;      ///<  人脸运动质量类型
  float score;         ///<  人脸运动质量类型得分
} SingleFaceNoiseMotionblur;

/// \brief 人脸运动质量结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceNoiseMotionblurResult : public Result {
 public:
  /// \brief 构造函数
  FaceNoiseMotionblurResult();

  /// \brief 析构函数
  ~FaceNoiseMotionblurResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceNoiseMotionblurs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceNoiseMotionblurs_.resize(n); }

 public:
  std::vector<SingleFaceNoiseMotionblur> faceNoiseMotionblurs_;  ///< 结果容器
};

/// \brief 人脸运动质量算法类
class ARCTERN_EXPORT_API FaceNoiseMotionblur : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceNoiseMotionblur();

  /// \brief 析构函数
  ~FaceNoiseMotionblur() override;

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
  AlgorithmBase *impl_ = nullptr;      ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLUR_H_
