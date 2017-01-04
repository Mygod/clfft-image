#ifndef PARALLEL_COMPUTING_PIXELARRAY_H
#define PARALLEL_COMPUTING_PIXELARRAY_H

#include <cassert>
#include <memory>

#include <png++/types.hpp>

namespace png {
template <class Pixel>
class row_traits<Pixel*> {
 public:
  static constexpr Pixel* get_data(Pixel* p) {
    return p;
  }
};
}


template <class Pixel>
class PixelArray {
 private:
  png::uint_32 width = 0, height = 0;
  std::unique_ptr<Pixel[]> pixels;
 public:
  typedef Pixel *row_type, *row_access;
  typedef Pixel const* row_const_access;
  typedef png::row_traits<Pixel*> row_traits;

  PixelArray() { }
  PixelArray(png::uint_32 width, png::uint_32 height)
      : width(width), height(height), pixels(new Pixel[width * height]) { }

  png::uint_32 get_width() const {
    return width;
  }
  png::uint_32 get_height() const {
    return height;
  }
  void resize(png::uint_32 width, png::uint_32 height) {
    if (this->width && this->height) assert(this->width == width && this->height == height);
    else if ((this->width = width) && (this->height = height)) pixels.reset(new Pixel[width * height]);
  }
  Pixel *get_row(size_t index) {
    return pixels.get() + width * index;
  }
  const Pixel *get_row(size_t index) const {
    return pixels.get() + width * index;
  }
  Pixel *operator[](size_t index) {
    return pixels.get() + width * index;
  }
  const Pixel *operator[](size_t index) const {
    return pixels.get() + width * index;
  }
  void put_row(size_t index, Pixel *row) {
    memcpy(pixels.get() + width * index, row, sizeof(Pixel) * width);
  }
  Pixel get_pixel(size_t x, size_t y) const {
    return pixels.get() + (width * y + x);
  }
  void set_pixel(size_t x, size_t y, Pixel p) {
    *(pixels.get() + (width * y + x)) = p;
  }
};


#endif //PARALLEL_COMPUTING_PIXELARRAY_H
