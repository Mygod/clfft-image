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
    for (auto platform : platforms) {
      vector<cl::Device> devices;
      platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
      cout << "Profile: " << platform.getInfo<CL_PLATFORM_PROFILE>() << endl
          << "Version: " << platform.getInfo<CL_PLATFORM_VERSION>() << endl
          << "Name: " << platform.getInfo<CL_PLATFORM_NAME>() << endl
          << "Vendor: " << platform.getInfo<CL_PLATFORM_VENDOR>() << endl
          << "Extensions: " << platform.getInfo<CL_PLATFORM_EXTENSIONS>() << endl
          << "Devices: " << devices.size() << endl;
      for (auto device : devices) cout
            << device.getInfo<CL_DEVICE_NAME>() << endl
            << "Hardware version: " << device.getInfo<CL_DEVICE_VERSION>() << endl
            << "Software version: " << device.getInfo<CL_DRIVER_VERSION>() << endl
            << "OpenCL C version: " << device.getInfo<CL_DEVICE_OPENCL_C_VERSION>() << endl
            << "Parallel compute units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl << endl;
    }
    return 0;
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return 1;
  }
}
