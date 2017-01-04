#ifndef PARALLEL_COMPUTING_FILEREADER_H
#define PARALLEL_COMPUTING_FILEREADER_H

#include <boost/filesystem.hpp>

class FileReader {
 public:
  static boost::filesystem::path baseDirectory;
  static inline void init(const char *basePath) {
    baseDirectory = boost::filesystem::path(basePath).parent_path();
  }
  static std::string read(boost::filesystem::path path);
};

#endif //PARALLEL_COMPUTING_FILEREADER_H
