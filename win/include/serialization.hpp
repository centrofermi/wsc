#ifndef WSC_SERIALIZATION_HPP
#define WSC_SERIALIZATION_HPP

#include <fstream>
#include <string>

namespace eee {
namespace wsc {

class OutputFile
{
  std::ofstream m_file;

 public:

  explicit
  OutputFile(std::string const& filename);

  std::streamoff write(double in_temp, double out_temp, double pressure);
};

} // ns wsc
} // ns eee

#endif // WSC_SERIALIZATION_HPP

