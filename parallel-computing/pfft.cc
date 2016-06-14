#define __CL_ENABLE_EXCEPTIONS

#include <iostream>

#include "CL/cl.hpp"

using namespace std;

int main(int argc, char **argv) {
  try {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto platform : platforms) cout << platform() << endl;
    return 0;
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return 1;
  }
}
