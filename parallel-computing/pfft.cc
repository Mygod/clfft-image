#include <iostream>

#include <png++/image.hpp>

#include "CLFFT.h"
#include "CLPlatform.h"
#include "FileReader.h"
#include "PixelArray.h"

using namespace std;
using imageType = png::image<png::rgb_pixel, PixelArray<png::rgb_pixel>>;

inline unsigned long pad(unsigned long length) {
  unsigned long n = length;
  for (;;) {
    while (!(n & 1)) n >>= 1;
    while (!(n % 3)) n /= 3;
    while (!(n % 5)) n /= 5;
    while (!(n % 7)) n /= 7;
    if (n == 1) return length;
    n = ++length;
  }
}

int main(int argc, char **argv) {
  shared_ptr<imageType> rawImage = make_shared<imageType>("input.png"), image;
  unsigned long w = rawImage->get_width(), h = rawImage->get_height(),
                width = pad(w), height = pad(h), size = width * height;
  if (w == width && h == height) image = rawImage; else {
    image = make_shared<imageType>(width, height);
    size_t l = static_cast<size_t>(w) * 3;
    for (unsigned long i = 0; i < height; ++i) memcpy(image->get_pixbuf()[i], image->get_pixbuf()[i], l);
  }
  FileReader::init(argv[0]);
  auto source = FileReader::read("kernel.cl");
  try {
    cl::FFT::setup();
    auto platform = CLPlatform::list()[1];
    auto context = platform.createContext();
    cl::Buffer bitmap(context, CL_MEM_READ_WRITE, 3 * size, image->get_pixbuf()[0]),
               temp(context, CL_MEM_READ_WRITE, size * sizeof(float) << 1);
    auto program = context.loadProgram(source);
#define KERNEL(x) cl::Kernel x(program, #x); x.setArg(0, bitmap); x.setArg(1, temp); x.setArg(2, (int) width)
    KERNEL(inputX);
    KERNEL(inputY);
    KERNEL(inputZ);
    KERNEL(outputX);
    KERNEL(outputY);
    KERNEL(outputZ);
#undef KERNEL
    auto device = context.devices[0];
    cl::CommandQueue queue(context, device);
    cl::FFT::Plan plan(context, width, height);
    plan.setPrecision(CLFFT_SINGLE);
    plan.setLayout(CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED);
    plan.setResultLocation(CLFFT_INPLACE);
    plan.setScale(CLFFT_FORWARD, 1.F / width / height); // since it's used for output

    queue.enqueueNDRangeKernel(inputX, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
    queue.enqueueNDRangeKernel(outputX, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    queue.enqueueNDRangeKernel(inputY, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
    queue.enqueueNDRangeKernel(outputY, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    queue.enqueueNDRangeKernel(inputZ, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
    queue.enqueueNDRangeKernel(outputZ, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
    queue.finish();
    assert(!plan.failFlag);
    assert(!context.failFlag);
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return -1;
  }
  rawImage->write("output.png");
  cl::FFT::teardown();
  return 0;
}
