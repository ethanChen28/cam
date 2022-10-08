/******************************************************************************
 * Copyright (c) 2017-2018 AiMall Corporation.
 * All Rights Reserved.
 * *****************************************************************************/

#ifndef _AIMALL_SDK_INFRA_CORE_ERR_H_
#define _AIMALL_SDK_INFRA_CORE_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

//imo sdk 接口返回错误定义
enum imo_err_code {
    IMO_API_RET_SUCCESS                 = 0,    //成功
    IMO_API_RET_INTERNAL_ERR            = -1,   //内部错误
    IMO_API_RET_SDK_NOT_INIT            = -2,   //SDK没有全局初始化
    IMO_API_RET_ALG_NOT_INIT            = -3,   //算法没有初始化

    IMO_API_RET_MODEL_PATH_ERR          = -10,  //模型路径错误

    IMO_API_RET_INVALID_KEY             = -20,  //不合法的授权key
    IMO_API_RET_PERMISSION_DENIED       = -21,  //当前key不具备算法的使用权限

    IMO_API_RET_PARAM_ERR               = -30,  //参数错误
};

#ifdef __cplusplus
}
#endif

#endif //_AIMALL_SDK_INFRA_DEFINE_H_
