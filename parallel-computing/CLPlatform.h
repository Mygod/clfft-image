#ifndef PARALLEL_COMPUTING_CLPLATFORM_H
#define PARALLEL_COMPUTING_CLPLATFORM_H

#include "boost/filesystem.hpp"

#include "CLContext.h"

class CLPlatform {
 public:
  static boost::filesystem::path baseDirectory;
  static void init(const char *basePath);
  static std::vector<CLPlatform> list();

  cl::Platform platform;
  std::vector<cl::Device> devices;

  CLPlatform(const cl::Platform &platform);
  inline CLContext createContext(std::vector<cl::Device> *devices = nullptr) const {
    return CLContext(devices ? *devices : this->devices);
  }

  inline operator cl::Platform() {
    return platform;
  }
  inline operator const cl::Platform() const {
    return platform;
  }
};

#endif //PARALLEL_COMPUTING_CLPLATFORM_H
