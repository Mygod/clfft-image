#include <iostream>

#include "CLPlatform.h"
#include "FileReader.h"

using namespace std;

int main(int argc, char **argv) {
  try {
    FileReader::init(argv[0]);
    auto source = FileReader::read("kernel.cl");
    auto platforms = CLPlatform::list();
    for (auto platform : platforms) if (!platform.devices.empty()) {
      auto context = platform.createContext();
      cl::Buffer buffer(context, CL_MEM_WRITE_ONLY, 16 * sizeof(char));
      auto program = context.loadProgram(source);
      cl::Kernel kernel(program, "hello_world");
      kernel.setArg(0, buffer);
      for (auto device : context.devices) {
        cl::CommandQueue queue(context, device);
        queue.enqueueTask(kernel);
        char result[16];
        queue.enqueueReadBuffer(buffer, CL_TRUE, 0, 16 * sizeof(char), result);
        cout << "Result from device " << device.getInfo<CL_DEVICE_NAME>() << ": " << result << endl;
      }
      assert(!context.failFlag);
    }
    return 0;
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return -1;
  }
}
