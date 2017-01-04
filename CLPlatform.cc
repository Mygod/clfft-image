#include "CLPlatform.h"

using namespace boost;
using namespace std;

vector<CLPlatform> CLPlatform::list() {
  vector<CLPlatform> result;
  vector<cl::Platform> raw;
  cl::Platform::get(&raw);
  result.assign(raw.begin(), raw.end());
  return result;
}

CLPlatform::CLPlatform(const cl::Platform &platform) : platform(platform) {
  platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
}
