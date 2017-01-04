#include "CLContext.h"

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost;
using namespace boost::posix_time;
using namespace std;

void pfnNotify(const char *errinfo, const void *private_info, size_t cb, void *user_data) {
  reinterpret_cast<CLContext*>(user_data)->failFlag = true;
  auto filename = to_iso_string(second_clock::local_time()) + ".dmp";
  ofstream(filename).write(reinterpret_cast<const char*>(private_info), cb);
  cerr << "Error occurred in OpenCL, details dumped to " << filename << ": " << errinfo << endl;
}
CLContext::CLContext(const vector<cl::Device> &devices) : devices(devices), context(devices, nullptr, pfnNotify, this) {
  assert(!devices.empty());
}

cl::Program CLContext::loadProgram(const string &source) const {
  cl::Program program(context, source);
  try {
    program.build(verbose ? "-Werror" : nullptr);
    if (verbose)
      cerr << "Build succeeded." << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices.front()) << endl;
  } catch (cl::Error error) {
    cerr << "Build failed with code " << error.err() << '.' << endl
         << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices.front()) << endl;
    exit(-2);
  }
  return program;
}


