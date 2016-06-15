#ifndef PARALLEL_COMPUTING_CLFFT_H
#define PARALLEL_COMPUTING_CLFFT_H

#include <CL/cl.hpp>
#include <clFFT.h>

namespace cl {
namespace FFT {
class InitHelper {
 public:
  InitHelper();
  ~InitHelper();
};

class Plan {
 private:
  clfftPlanHandle handle;
 public:
  inline Plan() : handle(0) { }
  Plan(cl::Context context, std::size_t x);
  Plan(cl::Context context, std::size_t x, std::size_t y);
  Plan(cl::Context context, std::size_t x, std::size_t y, std::size_t z);
  ~Plan();

  // TODO: wrap queues vector, events vector, etc. Maybe split bake?
  void enqueueTransform(clfftDirection dir, cl::CommandQueue queue, cl::Buffer inputBuffer = cl::Buffer(),
                        cl::Buffer outputBuffer = cl::Buffer(), cl::Buffer tmpBuffer = cl::Buffer());
  void setLayout(clfftLayout_ inLayout, clfftLayout_ outLayout);
  void setPrecision(clfftPrecision_ precision);
  void setResultLocation(clfftResultLocation_ placeness);
  void setScale(clfftDirection_ dir, cl_float scale);
};
}
}

#endif //PARALLEL_COMPUTING_CLFFT_H
