#ifndef WSC_COM_HPP
#define WSC_COM_HPP

#include <vector>
#include <string>

#include <asio.hpp>

namespace eee {
namespace wsc {

using ComPorts = std::vector<std::string>;
using ComPort  = asio::serial_port;

ComPorts enumerate();
ComPorts probe_filter(asio::io_context& ctx, ComPorts const& in);

} // ns wsc
} // ns eee

#endif // WSC_COM_HPP
