/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEMASK_FACEMASK_H_
#define SRC_ALGORITHMS_FACEMASK_FACEMASK_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸口罩运行参数类
class ARCTERN_EXPORT_API FaceMaskRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceMaskRunParameter();

  /// \brief 析构函数
  ~FaceMaskRunParameter();
};

/// \brief 佩戴口罩状态
typedef enum {
  MASK = 0,  ///<  佩戴口罩
  NOMASK = 1,  ///< 无佩戴口罩
} MaskStatus;

/// \brief 单人脸佩戴口罩信息
typedef struct {
  MaskStatus maskStatus;  ///<  口罩状态
  float statusScore;      ///<  口罩状态可信度得分
} SingleFaceMask;

/// \brief 人脸口罩结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceMaskResult : public Result {
 public:
  /// \brief 构造函数
  FaceMaskResult();

  /// \brief 析构函数
  ~FaceMaskResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { FaceMasks_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { FaceMasks_.resize(n); }

 public:
  std::vector<SingleFaceMask> FaceMasks_;  ///< 结果容器
};

/// \brief 人脸口罩算法类
class ARCTERN_EXPORT_API FaceMask : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceMask();

  /// \brief 析构函数
  ~FaceMask() override;

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
#endif  // SRC_ALGORITHMS_FACEMASK_FACEMASK_H_
