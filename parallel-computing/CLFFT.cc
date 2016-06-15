#include "CLFFT.h"

using cl::detail::errHandler;

#define __ERR_STR(x) #x

void cl::FFT::setup() {
  clfftSetupData setup;
  errHandler(clfftInitSetupData(&setup), __ERR_STR(clfftInitSetupData));
  errHandler(clfftSetup(&setup), __ERR_STR(clfftSetup));
}
void cl::FFT::teardown() {
  errHandler(clfftTeardown(), __ERR_STR(clfftTeardown));
}

cl::FFT::Plan::Plan(cl::Context context, std::size_t x) {
  errHandler(clfftCreateDefaultPlan(&handle, context(), CLFFT_1D, &x), __ERR_STR(clfftCreateDefaultPlan));
}
cl::FFT::Plan::Plan(cl::Context context, std::size_t x, std::size_t y) {
  std::size_t clLengths[2] = { x, y };
  errHandler(clfftCreateDefaultPlan(&handle, context(), CLFFT_2D, clLengths), __ERR_STR(clfftCreateDefaultPlan));
}
cl::FFT::Plan::Plan(cl::Context context, std::size_t x, std::size_t y, std::size_t z) {
  std::size_t clLengths[3] = { x, y, z };
  errHandler(clfftCreateDefaultPlan(&handle, context(), CLFFT_3D, clLengths), __ERR_STR(clfftCreateDefaultPlan));
}
cl::FFT::Plan::~Plan() {
  if (handle) errHandler(clfftDestroyPlan(&handle), __ERR_STR(clfftDestroyPlan));
}

void cl::FFT::Plan::enqueueTransform(clfftDirection dir, cl::CommandQueue queue,
                                     cl::Buffer inputBuffer, cl::Buffer outputBuffer, cl::Buffer tmpBuffer) {
  errHandler(clfftBakePlan(handle, 1, &(queue()), nullptr, nullptr), __ERR_STR(clfftBakePlan));
  errHandler(clfftEnqueueTransform(handle, dir, 1, &(queue()), 0, nullptr, nullptr,
                                   &(inputBuffer()), &(outputBuffer()), tmpBuffer()), __ERR_STR(clfftEnqueueTransform));
}
void cl::FFT::Plan::setLayout(clfftLayout_ inLayout, clfftLayout_ outLayout) {
  errHandler(clfftSetLayout(handle, inLayout, outLayout), __ERR_STR(clfftSetLayout));
}
void cl::FFT::Plan::setPrecision(clfftPrecision_ precision) {
  errHandler(clfftSetPlanPrecision(handle, precision), __ERR_STR(clfftSetPlanPrecision));
}
void cl::FFT::Plan::setResultLocation(clfftResultLocation_ placeness) {
  errHandler(clfftSetResultLocation(handle, placeness), __ERR_STR(clfftSetResultLocation));
}
void cl::FFT::Plan::setScale(clfftDirection_ dir, cl_float scale) {
  errHandler(clfftSetPlanScale(handle, dir, scale), __ERR_STR(clfftSetPlanScale));
}
