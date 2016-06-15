#ifndef PARALLEL_COMPUTING_CLCONTEXT_H
#define PARALLEL_COMPUTING_CLCONTEXT_H

#define __CL_ENABLE_EXCEPTIONS

#include "boost/filesystem.hpp"
#include "CL/cl.hpp"

class CLContext {
 public:
  cl::Context context;
  std::vector<cl::Device> devices;
  bool failFlag = false;

  CLContext(const std::vector<cl::Device> &devices);
  cl::Program loadProgram(const std::string &source) const;

  inline operator cl::Context() {
    return context;
  }
  inline operator const cl::Context() const {
    return context;
  }
};

#endif //PARALLEL_COMPUTING_CLCONTEXT_H
