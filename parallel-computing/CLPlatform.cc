#include "CLPlatform.h"

using namespace boost;
using namespace std;

filesystem::path CLPlatform::baseDirectory;
void CLPlatform::init(const char *basePath) {
  baseDirectory = filesystem::path(basePath).parent_path();
}

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
