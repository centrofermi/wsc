#include "serialization.hpp"

#include <ctime>
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace eee {
namespace wsc {

std::tm* localtime_b(std::time_t const* t, std::tm* ti)
{
#ifdef WSC_ON_WIN
  return std::localtime_s(t, ti);
#else
  return localtime_r(t, ti);
#endif
}

std::string mktime()
{
  std::time_t rawtime;
  std::time(&rawtime);

  char buffer[13]; // Just enough to contain '201801281520\0'
  std::tm timeinfo;
  localtime_b(&rawtime, &timeinfo);
  std::strftime(buffer, 13, "%Y%m%d%H%M", &timeinfo);

  return buffer;
}

OutputFile::OutputFile(std::string const& filename)
: m_file(filename.c_str())
{
  if (!m_file) {
    auto const e = errno;

    throw std::runtime_error(std::strerror(e));
  }
}

int OutputFile::write(double in_temp, double out_temp, double pressure)
{
  auto const beginning = m_file.tellp();
  // Column  0 - Date & Time;
  // Column  6 - In Temp;
  // Column  7 - Out Temp;
  // Column 23 - SL Barometer.

  m_file
    << mktime() << ",0,0,0,0,0,"
    << in_temp  << ','
    << out_temp << ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
    << (pressure - sea_level_pressure) << std::endl;

  return m_file.tellp() - beginning;
}

} // ns wsc
} // ns eee

