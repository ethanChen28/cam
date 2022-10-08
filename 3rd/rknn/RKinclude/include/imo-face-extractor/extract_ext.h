
#ifndef _AIMALL_ALG_SDK_FACE_EXTRACT_EXT_INCLUDE_HPP_
#define _AIMALL_ALG_SDK_FACE_EXTRACT_EXT_INCLUDE_HPP_

#include "extract.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 单帧图像中间结果
 */ 
typedef struct {
    float *  p;
}imo_face_extractor_IR;

/**
 * 堆上分配内存，如果分配成功，则需要主动释放，防止内存泄露
 * 
 * @param ir   中间结果数组
 * @param ir   数组大小
 * 
 * 0：内存分配成功； < 0 :参数错误
 */ 
int DLL_PUBLIC imo_face_extractor_createIR(imo_face_extractor_IR * ir, int size);


/**
 * 释放内存
 * 
 * @param ir   中间结果数组
 * @param ir   数组大小
 * 
 * 0：释放成功 ； < 0 :参数错误
 */
int DLL_PUBLIC imo_face_extractor_freeIR(imo_face_extractor_IR * ir, int size);


/**
 * 预处理，并把结果存储在 ir数组（线程安全）
 * 
 * @param  io_list 图片数组
 * @param  ir        [out]       中间结果数组  
 * @param  size      数组长度
 * 
 * 0：成功 ； <0 :失败，失败原因请查看返回值
 */ 
int DLL_PUBLIC
imo_face_extractor_preprocess(imo_face_extractor_io * io_list ,imo_face_extractor_IR * ir , int size ) ;


/**
 * 推理，并把结果赋值给io_list  (非线程安全)
 * 
 * @param  handle  句柄
 * @param  io_list  [in/out] 图片数组
 * @param  ir              中间结果数组  
 * @param  size      数组长度
 * 
 * 0：成功 ； <0 :失败，失败原因请查看返回值
 */ 
int DLL_PUBLIC
imo_face_extractor_inference(const imo_face_extractor_handle *handle, imo_face_extractor_io * io_list, imo_face_extractor_IR * ir ,int size ) ;


#ifdef __cplusplus
}
#endif

#endif //_AIMALL_ALG_SDK_FACE_EXTRACT_EXT_INCLUDE_HPP_
