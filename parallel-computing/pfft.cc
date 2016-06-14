#define __CL_ENABLE_EXCEPTIONS

#include <cassert>
#include <iostream>

#include "boost/core/ignore_unused.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/filesystem.hpp"
#include "CL/cl.hpp"

using namespace boost;
using namespace boost::posix_time;
using namespace std;

typedef vector<cl::Device> Devices;

void pfnNotify(const char *errinfo, const void *private_info, size_t cb, void *user_data) {
  ignore_unused(user_data);
  auto filename = to_iso_string(second_clock::local_time()) + ".dmp";
  ofstream(filename).write(reinterpret_cast<const char*>(private_info), cb);
  cerr << "Error occurred in OpenCL, details dumped to " << filename << ": " << errinfo << endl;
}

int main(int argc, char **argv) {
  try {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    assert(!platforms.empty());
    Devices devices;
    for (auto platform : platforms) {
      Devices thisDevices;
      platform.getDevices(CL_DEVICE_TYPE_ALL, &thisDevices);
      devices.insert(devices.end(), thisDevices.begin(), thisDevices.end());
    }
    assert(!devices.empty());
    assert(devices.size() == 2);
    devices.pop_back();
    cl::Context context(devices, nullptr, pfnNotify);

    filesystem::ifstream ifs(filesystem::absolute("cl/kernel.cl", filesystem::path(argv[0]).parent_path()), ios::ate);
    auto length = static_cast<streamoff>(ifs.tellg());
    ifs.seekg(0, ios::beg);
    char *programData = new char[length];
    ifs.read(programData, length);
    cl::Program program(context, string(move(programData)));
    try {
      program.build("-Werror");
    } catch (cl::Error error) {
      cerr << "Build failed with code " << error.err() << '.' << endl
           << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices.front()) << endl;
      return -2;
    }

    cl::Buffer buffer(context, CL_MEM_WRITE_ONLY, 32 * sizeof(char));
    cl::Kernel kernel(program, "hello_world");
    kernel.setArg(0, buffer);
    cl::CommandQueue queue(context, devices.front());
    queue.enqueueTask(kernel);
    char result[32];
    queue.enqueueReadBuffer(buffer, CL_TRUE, 0, 32 * sizeof(char), result);
    cout << "Result: " << result << endl;
    return 0;
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return -1;
  }
}
