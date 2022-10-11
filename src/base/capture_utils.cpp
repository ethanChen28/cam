#include "capture_utils.h"

#include <fstream>

#include "json.hpp"
namespace camera {
cv::Rect resizeRect(const cv::Rect &src, float wRatio, float hRatio) {
  cv::Rect dst{0, 0, 0, 0};
  dst.x = int(src.x * wRatio);
  dst.y = int(src.y * hRatio);
  dst.width = int(src.width * wRatio);
  dst.height = int(src.height * hRatio);
  return dst;
}
int parseConfigFile(const std::string &path, CaptureParam &param,
                    DetectParam &detectParam, TrackParam &trackParam,
                    UploadParam &uploadParam) {
  try {
    std::ifstream fin(path);
    if (!fin.is_open()) return -1;
    std::ostringstream tmp;
    tmp << fin.rdbuf();
    auto j3 = nlohmann::json::parse(tmp.str());
    if (!j3.contains("Detect")) return 1;
    if (!j3["Detect"].contains("ModelPath")) return 1;
    if (!j3["Detect"].contains("MinThresh")) return 1;
    if (!j3["Detect"].contains("MinRect")) return 1;
    if (!j3["Detect"].contains("Interv")) return 1;
    detectParam.modelPath = j3["Detect"]["ModelPath"].get<std::string>();
    detectParam.minThresh = j3["Detect"]["MinThresh"].get<float>();
    detectParam.minRect = j3["Detect"]["MinRect"].get<int>();
    detectParam.interv = j3["Detect"]["Interv"].get<int>();

    if (!j3.contains("Capture")) return 2;
    if (!j3["Capture"].contains("Width")) return 2;
    if (!j3["Capture"].contains("Height")) return 2;
    if (!j3["Capture"].contains("Mode")) return 2;
    if (!j3["Capture"].contains("Interv")) return 2;
    if (!j3["Capture"].contains("CaptureTime")) return 2;
    param.width = j3["Capture"]["Width"].get<int>();
    param.height = j3["Capture"]["Height"].get<int>();
    param.mode = j3["Capture"]["Mode"].get<int>();
    param.interv = j3["Capture"]["Interv"].get<int>();
    param.times = j3["Capture"]["CaptureTime"].get<std::vector<int>>();

    if (!j3.contains("Upload")) return 3;
    if (!j3["Upload"].contains("Ip")) return 3;
    if (!j3["Upload"].contains("Port")) return 3;
    uploadParam.ip = j3["Upload"]["Ip"].get<std::string>();
    uploadParam.port = j3["Upload"]["Port"].get<int>();

    if (!j3.contains("Track")) return 4;
    if (!j3["Track"].contains("Width")) return 4;
    if (!j3["Track"].contains("Height")) return 4;
    trackParam.width = j3["Track"]["Width"].get<int>();
    trackParam.height = j3["Track"]["Height"].get<int>();

  } catch (const std::exception &e) {
    std::cout << "parse config failed, stack: " << e.what() << std::endl;
    return 5;
  }
  return 0;
}

int CaculateWeekDay(int y, int m, int d) {
  if (m == 1 || m == 2) {
    m += 12;
    y--;
  }
  int iWeek = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
  // switch (iWeek)
  // {
  //     case 0: cout << "星期一" << endl; break;
  //     case 1: cout << "星期二" << endl; break;
  //     case 2: cout << "星期三" << endl; break;
  //     case 3: cout << "星期四" << endl; break;
  //     case 4: cout << "星期五" << endl; break;
  //     case 5: cout << "星期六" << endl; break;
  //     case 6: cout << "星期日" << endl; break;
  // }
  return iWeek;
}

bool isCaptureAtNow(const std::vector<int> &points) {
  time_t t;
  time(&t);

  auto gm = gmtime(&t);
  int year = 1900 + gm->tm_year;
  int month = 1 + gm->tm_mon;
  int day = gm->tm_mday;
  int hour = gm->tm_hour;

  int week = CaculateWeekDay(year, month, day);

  size_t hsum = week * 24 + hour;
  if (points.size() > hsum) {
    if (points[hsum] == 1) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
}  // namespace camera