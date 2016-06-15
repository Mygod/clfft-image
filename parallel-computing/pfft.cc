#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <png++/image.hpp>

#include "CLFFT.h"
#include "CLPlatform.h"
#include "FileReader.h"
#include "PixelArray.h"

using namespace std;
using boost::format;
using boost::ignore_unused;
namespace program_options = boost::program_options;

using ImageType = png::image<png::rgb_pixel, PixelArray<png::rgb_pixel>>;

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
  int platformIndex, deviceIndex;
  vector<string> input, output;
  FileReader::init(argv[0]);
  program_options::options_description od("Allowed options");
  od.add_options()("help,h", "produce this help message")
      ("platform,p", program_options::value(&platformIndex)->default_value(0), "OpenGL platform that one wishes to use")
      ("device,d", program_options::value(&deviceIndex)->default_value(0), "OpenGL device that one wishes to use")
      ("input,i", program_options::value(&input), "input PNG file(s) (-i, --input can be omitted)")
      ("output,o", program_options::value(&output), "output PNG file(s) (default = output{i}.png)");
  program_options::positional_options_description pod;
  pod.add("input", -1);
  program_options::variables_map vm;
  program_options::store(program_options::command_line_parser(argc, const_cast<const char *const *>(argv))
                             .options(od).positional(pod).run(), vm);
  program_options::notify(vm);
  if (input.empty() || vm.count("help")) {
    cout << od << endl;
    return 1;
  }

  try {
    cl::FFT::InitHelper _;
    ignore_unused(_);
    auto platform = CLPlatform::list()[platformIndex];
    auto context = platform.createContext();
    auto program = context.loadProgram(FileReader::read("kernel.cl"));
#define DEFINE_KERNEL(x) cl::Kernel x(program, #x)
    DEFINE_KERNEL(inputX);
    DEFINE_KERNEL(inputY);
    DEFINE_KERNEL(inputZ);
    DEFINE_KERNEL(outputX);
    DEFINE_KERNEL(outputY);
    DEFINE_KERNEL(outputZ);
#undef DEFINE_KERNEL
    auto device = context.devices[deviceIndex];
    cl::CommandQueue queue(context, device);

    int count = 0;
    for (auto path : input) {
      shared_ptr<ImageType> rawImage = make_shared<ImageType>(path), image;
      unsigned long w = rawImage->get_width(), h = rawImage->get_height(),
          width = pad(w), height = pad(h), size = width * height;
      if (w == width && h == height) image = rawImage; else {
        image = make_shared<ImageType>(width, height);
        size_t l = static_cast<size_t>(w) * 3, s = static_cast<size_t>(width - w) * 3;
        for (unsigned long i = 0; i < height; ++i) {
          memcpy(image->get_pixbuf()[i], rawImage->get_pixbuf()[i], l);
          memset(image->get_pixbuf()[i], 0, s);
        }
      }
      context.failFlag = false;
      cl::Buffer bitmap(context, CL_MEM_READ_WRITE, 3 * size),
          temp(context, CL_MEM_READ_WRITE, size * sizeof(float) << 1);
#define SETUP_KERNEL(x) x.setArg(0, bitmap); x.setArg(1, temp); x.setArg(2, (int) width)
      SETUP_KERNEL(inputX);
      SETUP_KERNEL(inputY);
      SETUP_KERNEL(inputZ);
      SETUP_KERNEL(outputX);
      SETUP_KERNEL(outputY);
      SETUP_KERNEL(outputZ);
#undef SETUP_KERNEL
      cl::FFT::Plan plan(context, width, height);
      plan.setPrecision(CLFFT_SINGLE);
      plan.setLayout(CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED);
      plan.setResultLocation(CLFFT_INPLACE);
      plan.setScale(CLFFT_FORWARD, 1.F / width / height); // since it's used for output

      queue.enqueueWriteBuffer(bitmap, CL_FALSE, 0, 3 * size, image->get_pixbuf()[0]);
      queue.enqueueNDRangeKernel(inputX, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
      queue.enqueueNDRangeKernel(outputX, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      queue.enqueueNDRangeKernel(inputY, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
      queue.enqueueNDRangeKernel(outputY, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      queue.enqueueNDRangeKernel(inputZ, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      plan.enqueueTransform(CLFFT_FORWARD, queue, temp);
      queue.enqueueNDRangeKernel(outputZ, cl::NullRange, cl::NDRange(width, height), cl::NDRange(1, 1));
      queue.enqueueReadBuffer(bitmap, CL_TRUE, 0, 3 * size, image->get_pixbuf()[0]);
      assert(!context.failFlag);
      image->write(count < output.size() ? output[count] : (format("output%d.png") % count).str());
      ++count;
    }
  } catch (cl::Error error) {
    cerr << error.what() << " failed with code " << error.err() << endl;
    return -1;
  }
  return 0;
}
