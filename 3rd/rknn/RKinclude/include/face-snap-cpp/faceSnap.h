#ifndef _IMO_FACE_SNAP_API_H
#define _IMO_FACE_SNAP_API_H

#include "imo-core/imo_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************IMO-SNAP-DEFINE***********************************/

/**
* 人脸抓拍检测-handle
*/
typedef struct {
    /**
     * 检测句柄
     */
    void *detect_handle;
    /**
     * 跟踪句柄
     */
    void *tracker_handle;
} imo_face_snap_handle;

typedef struct {
    /**
     * 最小人脸尺寸，未来可能会舍弃此参数
     */
    int min_face_size;
#if defined(__ANDROID__) || defined(__APPLE__)
    /**
     * 检测人脸点位时是否开启高质量检测，默认true
     */
    bool predict_high_quality_points;
#endif

    /**
     * 底层跑检测网络时的并发线程数，默认4，主要针对嵌入式，手机等场景，调节此参数，推理速度可能会有变化
     */
    int num_threads;
  
    /**
     * 最大GPU限制，默认为0.3, 此参数主要用于nvidia GPU.
     */
    float max_gpu;
    /**
     * 阈值，用于过滤框，默认是0.45
     */
    float thresh;

    /**
     * 是否使用异步模式
     */
    bool async;

} imo_face_snap_config;

/**
 * 人脸检测-单个人脸信息
 */
typedef struct {
    /**
     * 人脸框
     */
    imo_rect rect;

    /**
     * confidence of rect
     */
    float  score;

    /*
    * 人脸ID
    */
   int id;
} imo_face_snap_face_info;

/**
 * 人脸检测-多人脸信息
 */
typedef struct {

    imo_image img;                   // image 图片数据

    imo_image_orientation ori;       //orientation 图片中人头的朝向 IMO_IMAGE_UP

    /**
     * 人脸信息数组 (in & out)
     */
    imo_face_snap_face_info *face_infos;

    // 客户端定义最大检测的人脸数，face_infos数组的长度 (in)
    unsigned int input_size;

    // 检测到的人脸个数 (out)
    unsigned int output_size;
} imo_face_snap_io;



/***********************************IMO-API***********************************/

/**
 * 人脸抓拍初始化
 * @param handle 句柄（out）
 * @param config 参数配置，默认填nullptr即可
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_face_snap_create(imo_face_snap_handle *handle, const imo_face_snap_config *config);

/**
 * 人脸抓拍-检测图片
 * @param handle 句柄（由imo_face_snap_create接口创建）
 * @param ios  输入输出数组。　一个io 也就是一个待检测图片和图片相关的人脸信息结构体（一个图片上可能有多个人脸）
 * @param size  ios数组大小
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_face_snap_exec(const imo_face_snap_handle *handle, imo_face_snap_io * ios , int size, const imo_face_snap_config *snap_config);


/**
 * 人脸抓拍-销毁句柄
 * @param handle 句柄（由imo_face_snap_create接口创建）
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_face_snap_destroy(imo_face_snap_handle *handle);

#ifdef __cplusplus
}
#endif

#endif //_IMO_FACE_SNAP_API_H
