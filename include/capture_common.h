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
#include <sstream>
#include <iostream>
#include <thread>
#include <uuid/uuid.h>
template <class T>
T string2T(const std::string &str) {
  std::stringstream stream;
  stream << str;
  T result;
  stream >> result;
  return result;
}

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
  std::string ret = "";
  if (path == "") return ret;

  auto pos = path.find_last_of('/');
  if (pos == std::string::npos) {
    ret = path;
  } else {
    ret = path.substr(pos + 1);
  }
  pos = ret.find_last_of('.');
  if(pos == std::string::npos){
    return ret;
  }
  return ret.substr(0, pos);
}

inline bool isExsit(const std::string &path) {
  if (access(path.c_str(), 0)) {
    return false;
  }
  return true;
}

inline std::string getSn() {
  std::string stn = "";
  std::string stnFile = "/stn.conf";

  if (isExsit(stnFile)) {
    std::fstream fin(stnFile, std::ios::in);
    if (fin.is_open()) {
      fin >> stn;
      fin.close();
    }
  }
  if (stn.empty()) {
    uuid_t uid;
    uuid_generate(uid);

    char element[3];
    for (size_t i = 0; i < sizeof(uid); i++) {
      sprintf(element, "%02X", uid[i]);
      // if(i!=0) stn += "-";
      stn += std::string(element, 2);
    }

    std::fstream fout(stnFile, std::ios::out);
    fout << stn;
    fout.close();
  }
  return stn;
}
