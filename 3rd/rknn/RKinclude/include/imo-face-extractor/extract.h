#ifndef _AIMALL_ALG_SDK_FACE_EXTRACT_INCLUDE_HPP_
#define _AIMALL_ALG_SDK_FACE_EXTRACT_INCLUDE_HPP_

#include "imo-core/imo_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMO_FACE_EXTRACTOR_FACE_INFO_POINTS_COUNT    5
#define IMO_FACE_FEATURE_SIZE                       256


typedef struct {

    /**
     * 底层跑检测网络时的并发线程数，默认4,主要针对嵌入式，手机等场景，调节此参数，推理速度可能会有变化
     */
    int num_threads;

    /**
     * 最大GPU使用率，默认是0.3，针对Nvidia GPU使用场景
     */
    float max_gpu;
} imo_face_extractor_config;
/**
* 人脸特征提取-handle
*/
typedef struct {
    /**
     * 句柄
     */
    void *handle;
} imo_face_extractor_handle;

/**
 * 人脸特征提取-io
 */
typedef struct {
    /**
     * 输入图片
     */
    imo_image in_mat;
    /**
     * 图片中人脸的5点信息(左眼/右眼/鼻尖/左嘴角/右嘴角)
     */
    imo_point points[IMO_FACE_EXTRACTOR_FACE_INFO_POINTS_COUNT];
    /**
     * 人脸特征值数据
     */
    float out_feature[IMO_FACE_FEATURE_SIZE];
    /**
     * 人脸特征值质量,暂时未用到
     */
    float out_feature_quality;
} imo_face_extractor_io;


/**
 * 人脸特征提取初始化
 * @param handle 句柄（out）
 * @param config 配置信息，可为NULL(in)
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_face_extractor_create(imo_face_extractor_handle *handle, imo_face_extractor_config * config);

/**
 * 人脸特征提取-销毁句柄
 * @param handle 句柄（由imo_face_extractor_create接口创建）
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_face_extractor_destroy(imo_face_extractor_handle *handle);

/**
 * 人脸特征提取
 * @param handle 句柄（由imo_face_extractor_create接口创建）
 * @param io_list io数组
 * @param size io_list数组元素个数
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC
imo_face_extractor_exec(const imo_face_extractor_handle *handle, imo_face_extractor_io *io_list, int size);

/**
 * 人脸特征值比对1v1
 * @param feature1 特征值1
 * @param feature2 特征值2
 * @param score 相似度得分
 */
int DLL_PUBLIC
imo_face_extractor_compare(float feature1[IMO_FACE_FEATURE_SIZE], float feature2[IMO_FACE_FEATURE_SIZE], float *score);


#ifdef __cplusplus
}
#endif


#endif //_AIMALL_ALG_SDK_FACEU_EXTRACT_INCLUDE_H_
