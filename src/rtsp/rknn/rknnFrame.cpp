#include "rknnFrame.h"

#include "rkmedia_api.h"
#include "sample_common.h"

#include <thread>
#include "capture_common.h"
namespace camera {

RknnFrame::RknnFrame(/* args */) {}

RknnFrame::~RknnFrame() {}

int RknnFrame::get(FUNC &callBack) {
  RK_U32 u32Width = 1920;
  RK_U32 u32Height = 1080;
  int frameCnt = 10;
  RK_CHAR *pDeviceName = (RK_CHAR *)"rkispp_scale0";
  RK_CHAR *pIqfilesPath = (RK_CHAR *)"/oem/etc/iqfiles/";
  RK_S32 s32CamId = 0;
  RK_BOOL bMultictx = RK_FALSE;


  printf("#####Device: %s\n", pDeviceName);
  printf("#####Resolution: %dx%d\n", u32Width, u32Height);
  printf("#####Frame Count to save: %d\n", frameCnt);
  printf("#CameraIdx: %d\n\n", s32CamId);

  if (pIqfilesPath) {
    printf("#####Aiq xml dirpath: %s\n\n", pIqfilesPath);
    printf("#bMultictx: %d\n\n", bMultictx);
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    int fps = 30;
    SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, pIqfilesPath);
    SAMPLE_COMM_ISP_Run(s32CamId);
    SAMPLE_COMM_ISP_SetFrameRate(s32CamId, fps);
  }

  RK_MPI_SYS_Init();
  VI_CHN_ATTR_S vi_chn_attr;
  vi_chn_attr.pcVideoNode = pDeviceName;
  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = u32Width;
  vi_chn_attr.u32Height = u32Height;
  vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
  vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
  auto ret = RK_MPI_VI_SetChnAttr(s32CamId, 0, &vi_chn_attr);
  ret |= RK_MPI_VI_EnableChn(s32CamId, 0);
  if (ret) {
    printf("Create VI[0] failed! ret=%d\n", ret);
    return -1;
  }

  printf("%s initial finish\n", __func__);

  code_.store(0);
  auto thread_handle = std::make_shared<std::thread>(&RknnFrame::GetMediaBuffer, this,
                                                     std::ref(callBack));
  thread_handle->detach();
  ret = RK_MPI_VI_StartStream(s32CamId, 0);
  if (ret) {
    printf("Start VI[0] failed! ret=%d\n", ret);
    return -1;
  }

  while (true) {
    if(code_.load() != 0) break;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }

  RK_MPI_VI_DisableChn(s32CamId, 0);

  if (pIqfilesPath)
    SAMPLE_COMM_ISP_Stop(s32CamId);

  printf("%s exit!\n", __func__);
  return 0;

}

void RknnFrame::GetMediaBuffer(FUNC &func) {
  MEDIA_BUFFER mb = NULL;
  while (true) {
    mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VI, 0, -1);
    if (!mb) {
      std::cout << "RK_MPI_SYS_GetMediaBuffer get null buffer!" << std::endl;
      code_.store(1);
      return;
    }

    MB_IMAGE_INFO_S stImageInfo = {0};
    int ret = RK_MPI_MB_GetImageInfo(mb, &stImageInfo);
    if (ret) {
      std::cout << "Warn: Get image info failed! ret = " <<  ret << std::endl;
      code_.store(2);
      return;
    }

    FrameInfo frame;
    frame.data = (char*) RK_MPI_MB_GetPtr(mb);
    frame.size = RK_MPI_MB_GetSize(mb);
    frame.height = stImageInfo.u32Height;
    frame.width = stImageInfo.u32Width;
    frame.time = getCurrentTime();
    frame.fmt = 0;

    func(frame);

    RK_MPI_MB_ReleaseBuffer(mb);
  }
}
}  // namespace camera