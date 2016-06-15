#include "FileReader.h"

using namespace boost;
using namespace std;

filesystem::path FileReader::baseDirectory;
string FileReader::read(filesystem::path path) {
  if (path.is_relative()) path = filesystem::absolute(path, baseDirectory);
  filesystem::ifstream ifs(path, ios::ate);
  assert(!ifs.fail());
  auto length = static_cast<streamoff>(ifs.tellg());
  ifs.seekg(0, ios::beg);
  char *programData = new char[length];
  ifs.read(programData, length);
  return string(move(programData));
}
