#pragma once
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/wait.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
inline time_t getCurrentTime() {
  time_t t;
  time(&t);
  return t;
}

inline std::string timeToString(const time_t &t) {
  auto ptm = localtime(&t);

  auto mon = std::to_string(ptm->tm_mon + 1);
  mon = mon.size() == 1 ? "0" + mon : mon;
  auto day = std::to_string(ptm->tm_mday);
  day = day.size() == 1 ? "0" + day : day;
  auto hour = std::to_string(ptm->tm_hour);
  hour = hour.size() == 1 ? "0" + hour : hour;
  auto min = std::to_string(ptm->tm_min);
  min = min.size() == 1 ? "0" + min : min;
  auto sec = std::to_string(ptm->tm_sec);
  sec = sec.size() == 1 ? "0" + sec : sec;

  std::string result = std::to_string(ptm->tm_year + 1900) + "-" + mon + "-" +
                       day + " " + hour + ":" + min + ":" + sec;

  return result;
}

inline std::string getIpByName(const std::string &name) {
  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    return "";
  }

  char buf[512];
  struct ifconf ifconf;
  ifconf.ifc_len = 512;
  ifconf.ifc_buf = buf;

  ioctl(sockfd, SIOCGIFCONF, &ifconf);

  auto ifreq = (struct ifreq *)ifconf.ifc_buf;

  for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--) {
    // for ipv4
    if (ifreq->ifr_flags == AF_INET) {
      std::string ip =
          inet_ntoa(((struct sockaddr_in *)&(ifreq->ifr_addr))->sin_addr);
      std::string tmpName = ifreq->ifr_name;
      if (tmpName == name) {
        return ip;
      }
      ifreq++;
    }
  }
  return "";
}

inline std::string getNameFromPath(const std::string &path) {
  if (path == "") return "";
  auto pos = path.find_last_of('/');
  if (pos == std::string::npos) return path;
  return path.substr(pos + 1);
}