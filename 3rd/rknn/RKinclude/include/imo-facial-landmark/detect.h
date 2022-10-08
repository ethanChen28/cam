#ifndef _IMO_FACIAL_LANDMARK_API_H
#define _IMO_FACIAL_LANDMARK_API_H

#include "imo-core/imo_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************IMO-DETECT-DEFINE***********************************/

// 人脸关键点检测结果的点位数量
#define IMO_FACE_DETECTOR_FACE_INFO_POINTS_COUNT    5

/**
* 人脸关键点检测-handle
*/
typedef struct {
    /**
     * 句柄
     */
    void *handle;
} imo_facial_landmark_handle;

typedef struct {

    /**
     * 底层跑检测网络时的并发线程数，默认4，主要针对嵌入式，手机等场景，调节此参数，推理速度可能会有变化
     */
    int num_threads;
  
    /**
     * 最大GPU限制，默认为0.3, 此参数主要用于nvidia GPU.
     */
    float max_gpu;

} imo_facial_landmark_config;

/**
 * 人脸关键点检测-单个人脸信息
 */
typedef struct {
    /**
     * 人脸点位[X1,Y1,X2,Y2,...,X5, Y5](左眼/右眼/鼻尖/左嘴角/右嘴角)
     */
    imo_point points[IMO_FACE_DETECTOR_FACE_INFO_POINTS_COUNT];

    /**
     * confidence of face
     */
    float  faceScore;
} imo_facial_landmark_face_info;

/**
 * 人脸关键点检测-多人脸信息
 */
typedef struct {

    imo_image img;                   // image 图片数据

    imo_image_orientation ori;       //orientation 图片中人头的朝向 IMO_IMAGE_UP

    /**
     * 人脸信息数据
     */
    imo_facial_landmark_face_info face_infos;

} imo_facial_landmark_io;



/***********************************IMO-API***********************************/

/**
 * 人脸关键点检测初始化
 * @param handle 句柄（out）
 * @param config 参数配置，默认填nullptr即可
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_facial_landmark_create(imo_facial_landmark_handle *handle, const imo_facial_landmark_config *config);

/**
 * 人脸关键点检测-检测图片
 * @param handle 句柄（由imo_facial_landmark_create接口创建）
 * @param ios  输入输出数组。　一个io 也就是一个待检测图片和图片相关的人脸信息结构体（一个图片上可能有多个人脸）
 * @param size  ios数组大小
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_facial_landmark_exec(const imo_facial_landmark_handle *handle, imo_facial_landmark_io * ios , int size);


/**
 * 人脸关键点检测-销毁句柄
 * @param handle 句柄（由imo_facial_landmark_create接口创建）
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_facial_landmark_destroy(imo_facial_landmark_handle *handle);

#ifdef __cplusplus
}
#endif

#endif //_IMO_FACIAL_LANDMARK_API_H
