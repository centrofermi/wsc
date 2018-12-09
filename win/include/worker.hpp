#ifndef WSC_WORKER_HPP
#define WSC_WORKER_HPP

#include <string>
#include <functional>
#include <thread>
#include <com.hpp>

namespace eee {
namespace wsc {

class Worker
{
  asio::io_context m_ctx;
  std::thread m_thread;

 public:

  using callback = std::function<void(std::string const& msg)>;

  bool running() const
  {
    return !m_ctx.stopped();
  }

  void stop();

  void run(
      std::string const& port
    , std::string const& output_file
    , callback const& data_cb
    , callback const& error_cb
  );

  ~Worker()
  {
    stop();

    if (m_thread.joinable()) {
      m_thread.join();
    }
  }
};

} // wsc
} // eee

#endif // WSC_WORKER_HPP
