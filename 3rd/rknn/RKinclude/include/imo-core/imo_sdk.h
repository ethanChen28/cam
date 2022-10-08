

#ifndef _AIMALL_SDK_INFRA_CORE_SDK_H_
#define _AIMALL_SDK_INFRA_CORE_SDK_H_

#include "imo-core/imo_define.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
#ifdef __ANDROID__
    /**
     * 授权超时时间
     */
    int auth_timeout_millis;
#endif
} imo_sdk_config;

/**
 * SDK全局初始化接口，必须在所有的SDK使用前调用
 * @param model_path  模型文件目录(android平台下，不为空时从相应路径读取模型，为空时从assets中读取模型，其他平台不能为空)
 * @param key       imo密钥
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_sdk_init(const char* model_path, const char* key);

/**
 * SDK全局初始化接口，必须在所有的SDK使用前调用
 * @param model_path  模型文件目录(android平台下，不为空时从相应路径读取模型，为空时从assets中读取模型，其他平台不能为空)
 * @param key       imo密钥
 * @param config    初始化配置
 * @return >=0:成功 <0:失败
 */
int DLL_PUBLIC imo_sdk_init_ex(const char* model_path, const char* key, const imo_sdk_config* config);

/**
 * SDK全局反初始化接口，在所有的SDK停止使用后调用
 */
int DLL_PUBLIC imo_sdk_deinit();

/**
 * 设置日志静默模式，默认是静默状态。如果调试状态下，需要打开log,请设置enable = 0
 */
void DLL_PUBLIC imo_log_set_quiet(int enable);

#ifdef __cplusplus
}
#endif

#endif //_AIMALL_SDK_INCLUDE_ENCTYPTION_DECODE_H_
