#include "../base/Exception.h"
#include "../base/Logger.h"

namespace BR2 {
Exception::Exception(const string_t& c, int line, const char* file, bool bLogError) {
  _line = line;
  _msg = c;
  _file = file;
  if (bLogError) {
    string_t err = c + " " + file + ":" + line;
    BRLogError(err);
  }

  Gu::debugBreak();
}
string_t Exception::what() const {
  return _msg + " file:" + _file + " line:" + _line;
}

}  // namespace BR2
