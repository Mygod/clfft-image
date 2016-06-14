#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <cassert>

#include "CL/cl.hpp"

using namespace std;

int main(int argc, char **argv) {
  try {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    assert(!platforms.empty());
    auto platform = platforms.front();
    vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    assert(!devices.empty());
    for (auto device : devices) cout
          << device.getInfo<CL_DEVICE_NAME>() << endl
          << "Hardware version: " << device.getInfo<CL_DEVICE_VERSION>() << endl
          << "Software version: " << device.getInfo<CL_DRIVER_VERSION>() << endl
          << "OpenCL C version: " << device.getInfo<CL_DEVICE_OPENCL_C_VERSION>() << endl
          << "Parallel compute units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl << endl;
    return 0;
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return 1;
  }
}
