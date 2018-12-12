#ifndef WSC_SERIALIZATION_HPP
#define WSC_SERIALIZATION_HPP

#include <fstream>

namespace eee {
namespace wsc {

class OutputFile
{
  std::ofstream m_file;

 public:

  auto static constexpr sea_level_pressure = 101325; // Pa

  explicit
  OutputFile(std::string const& filename);

  int write(double in_temp, double out_temp, double pressure);
};

} // ns wsc
} // ns eee

#endif // WSC_SERIALIZATION_HPP

