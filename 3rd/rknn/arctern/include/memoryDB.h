/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MEMORYDB_MEMORYDB_H_
#define SRC_ALGORITHMS_MEMORYDB_MEMORYDB_H_

#include <vector>
#include <list>
#include <memory>
#include "include/arctern.h"
#include "include/algorithminterface.h"

namespace arctern {
  
/// \brief 计算两组特征值的相似度
/// \param encfeature1 - 第一组加密的特征值
/// \param encfeature2 - 第二组加密的特征值
ARCTERN_EXPORT_API float  compute_similarity(const std::vector<uint8_t> &encfeature1,
                         const std::vector<uint8_t> &encfeature2);

/// \brief 特征数据库接口类
class ARCTERN_EXPORT_API MemoryDB {
 public:
  /// \brief 构造函数
  MemoryDB();

  /// \brief 析构函数
  ~MemoryDB();

  /// \brief 数据库初始化
  /// \param cap - Capacity of block unit
  /// \param dim - Dimension of feature, normally is 512, notice: (dim*4)+16=feature.size().
  /// \return 错误码
  ErrorType Init(size_t cap, size_t dim);  

  /// \brief 插入元素
  /// \param id - id
  /// \param feature - 特征值
  /// \return 错误码
  ErrorType feature_memdb_insert(int64_t id, const std::vector<uint8_t>& feature);


  /// \brief  Adds a batch of \a feature with \a id.
  /// \param  id - A batch of ids.
  /// \param  feature - A batch of features, note that this is an array of a
  ///                   vector of uint8_t, and size is id.size().
  /// \return 错误码
  ErrorType feature_memdb_insert(const std::vector<int64_t>& id,
                           const std::vector<uint8_t>* feature);


  /// \brief  Erases a feature according to \a id
  /// \param  id - To be erased.
  /// \return  0 - Success.
  /// \return 错误码
  ErrorType feature_memdb_erase(int64_t id);

  /// \brief memdb 清空
  void feature_memdb_clear();


  /// \brief  update a \a feature with \a id.
  /// \param  id - id of feature need updating.
  /// \param  feature - new feature to update the feature in database.
  /// \return 错误码
  ErrorType feature_memdb_update(int64_t id, const std::vector<uint8_t>& feature);


  /// \brief  Compares \a feature with memory db.
  /// \param  feature - feature to be compared.
  /// \param  similarity - output of similarity,and size is memory db size().
  /// \return 错误码
  ErrorType feature_memdb_compare(const std::vector<uint8_t>& feature,
                            std::vector<float>& similarity) const;


  /// \brief  Compares \a feature with memory db.
  /// \param  feature - feature to be compared.
  /// \param  similarity - output of similarity,and size is memory db size().
  /// \return 错误码
  ErrorType feature_memdb_compare(const std::list<std::vector<uint8_t>>& feature,
                            std::list<std::vector<float>>& similarity) const;

  /// \brief 定位到数据库中的位置
  /// \param idx - id序号
  /// \return -1表示序号越界，非负数表示定位的位置。
  int64_t feature_memdb_at(size_t idx) const;

  /// \brief memdb 是否为空
  /// \return true: 空
  bool feature_memdb_empty() const;

  /// \brief memdb 元素数量
  /// \return 数量
  size_t feature_memdb_size() const;

  /// \brief  Computes mean feature according to \a `feature` and the feature
  ///         where id = \a `id`. Returns feature after merging.
  /// \param  id - which id to be updated.
  /// \param  feature - the feature to be merged.
  /// \return the feature after merging. Empty vector if something was wrong
  std::vector<uint8_t> feature_memdb_mean_merge(int64_t id,
      const std::vector<uint8_t>& feature);
  
  /// \brief 释放资源
  void Release();

 private:
  class Impl;
  std::shared_ptr<Impl> impl_; ///< 算法实现类
};

}  // namespace arctern
#endif  // SRC_ALGORITHMS_MEMORYDB_MEMORYDB_H_
