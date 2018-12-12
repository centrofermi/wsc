#include <thread>
#include <chrono>
#include <regex>
#include <system_error>

#include "worker.hpp"

#include "serialization.hpp"

namespace eee {
namespace wsc {

std::string getline(asio::streambuf& buffer)
{
  std::istream stream(&buffer);
  std::string line;
  std::getline(stream, line);
  return line;
}

using buf_iterator = asio::buffers_iterator<asio::streambuf::const_buffers_type>;

std::pair<buf_iterator, bool> match_three_lines(
    buf_iterator begin, buf_iterator end
) {
  auto count = 0;
  auto it = begin;
  while (count != 3) {
    it = std::find(it, end, '\r');

    if (it == end) {
      break;
    }

    ++it;

    if (it == end) {
      break;
    }

    if (*it == '\n') {
      ++count;
    }
  }

  if (count == 3) {
    return std::make_pair(++it, true);
  } else {
    return std::make_pair(begin, false);
  }
}

class Connection
{
  ComPort m_port;
  OutputFile m_outputfile;
  asio::steady_timer m_timer;
  Worker::callback m_data_cb;
  Worker::callback m_error_cb;
  asio::streambuf m_buffer;

  void put_timer()
  {
    using namespace std::placeholders;
    using namespace std::chrono_literals;

    m_timer.expires_from_now(1s);
    m_timer.async_wait(std::bind(&Connection::timer_callback, this, _1));
  }

  void read_callback(
      std::error_code const& ec
    , std::size_t read_bytes
  ) {
    if (
         ec == std::errc::operation_canceled
      || ec == asio::error::operation_aborted
    ) {
      return;
    } else if (ec) {
      m_error_cb(ec.message());
      return;
    } else {
      auto const in_temp_line = getline(m_buffer);
      auto const pressure_line = getline(m_buffer);
      auto const humidity_line = getline(m_buffer);

      m_data_cb(in_temp_line);
      m_data_cb(pressure_line);
      m_data_cb(humidity_line);

      auto const in_temp = std::stof(in_temp_line.substr(3));
      auto const pressure = std::stof(pressure_line.substr(3));

      m_outputfile.write(in_temp, in_temp, pressure);

      put_timer();
    }
  }

  void timer_callback(std::error_code const& ec)
  {
    if (
         ec == std::errc::operation_canceled
      || ec == asio::error::operation_aborted
    ) {
      return;
    } else if (ec) {
      m_error_cb(ec.message());
      return;
    } else {
      char const command[] = "tph";
      std::error_code ec;
      asio::write(m_port, asio::buffer(command, sizeof(command) - 1), ec);

      if (ec) {
        m_error_cb(ec.message());
        return;
      } else {
        using namespace std::placeholders;

        asio::async_read_until(
            m_port
          , m_buffer
          , match_three_lines
          , std::bind(&Connection::read_callback, this, _1, _2)
        );
      }
    }
  }

 public:

  Connection(
      asio::io_context& ctx
    , std::string const& port
    , std::string const& output_file
    , Worker::callback const& data_cb
    , Worker::callback const& error_cb
  ) : m_port(ctx)
    , m_outputfile(output_file)
    , m_timer(ctx)
    , m_data_cb(data_cb)
    , m_error_cb(error_cb)
  {
    std::error_code ec;
    m_port.open(port, ec);

    if (ec) {
      error_cb(ec.message());
      throw ec;
    }

    ctx.reset();
    init_arduino(m_port);

    put_timer();
  }
};

void Worker::run(
    std::string const& port
  , std::string const& output_file
  , Worker::callback const& data_cb
  , Worker::callback const& error_cb
) {
  if (m_thread.joinable()) {
    m_thread.join();
  }

  std::thread th(
    [&, port, output_file, data_cb, error_cb]() noexcept {
      try {
        Connection c(m_ctx, port, output_file, data_cb, error_cb);

        m_ctx.reset();
        m_ctx.run();
      } catch (std::exception const& e) {
        error_cb(e.what());
        return;
      }
    }
  );

  std::swap(m_thread, th);
}

void Worker::stop()
{
  if (running()) {
    m_ctx.stop();
  }
}

} // wsc
} // eee
