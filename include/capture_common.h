#pragma once
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <fstream>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/statfs.h>

#include <ctime>
#include <iostream>
inline std::string getCurrentTime() {
  time_t t;
  time(&t);
  auto ptm = localtime(&t);
  std::string result =
      std::to_string(ptm->tm_year + 1900) + "-" +
      std::to_string(ptm->tm_mon + 1) + "-" + std::to_string(ptm->tm_mday) +
      " " + std::to_string(ptm->tm_hour) + ":" + std::to_string(ptm->tm_min) +
      ":" + std::to_string(ptm->tm_sec);

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
      std::string ip = inet_ntoa(((struct sockaddr_in *)&(ifreq->ifr_addr))->sin_addr);
      std::string tmpName = ifreq->ifr_name;
      if(tmpName == name){
        return ip;
      }
      ifreq++;
    }
  }
  return "";
}