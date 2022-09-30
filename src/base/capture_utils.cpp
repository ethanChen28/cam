#include "capture_common.h"
namespace camera {
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
bool isCaptureAtNow(const std::vector<int> & points) {
  time_t t;
  time(&t);

  auto gm = gmtime(&t);
  int year = 1900 + gm->tm_year;
  int month = 1 + gm->tm_mon;
  int day = gm->tm_mday;
  int hour = gm->tm_hour;

  int week = CaculateWeekDay(year, month, day);

  int hsum = week * 24 + hour;
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