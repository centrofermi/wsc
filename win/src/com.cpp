#include <chrono>
using namespace std::chrono_literals;

#include <Windows.h>
#include <SetupAPI.h>
#pragma comment(lib, "Setupapi.lib")

#include "com.hpp"

namespace eee {
namespace wsc {

constexpr int speed = 9600;

ComPorts enumerate()
{
  ComPorts names;

  DWORD size;
  GUID guid[1];

  BOOL const rv = SetupDiClassGuidsFromNameA(
      "Ports", (LPGUID) &guid
    , 1, &size
  );

  if (!rv) {
    return names;
  }

  HDEVINFO const hdevinfo = SetupDiGetClassDevs(
      &guid[0]
    , NULL, NULL
    , DIGCF_PRESENT | DIGCF_PROFILE
  );

  if (hdevinfo == INVALID_HANDLE_VALUE) {
    return names;
  }

  DWORD idx = 0;
  SP_DEVINFO_DATA devinfo_data;
  devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);

  while (SetupDiEnumDeviceInfo(hdevinfo, idx++, &devinfo_data)) {
    char friendly_name[MAX_PATH];
    char port_name[MAX_PATH];
    DWORD prop_type;
    DWORD type = REG_SZ;

    {
      BOOL const rv = ::SetupDiGetDeviceRegistryProperty(
          hdevinfo, &devinfo_data, SPDRP_FRIENDLYNAME, &prop_type
        , (LPBYTE) friendly_name, sizeof(friendly_name), &size
      );

      if (!rv) {
        continue;
      }
    }

    HKEY const hKey = ::SetupDiOpenDevRegKey(
        hdevinfo, &devinfo_data
      , DICS_FLAG_GLOBAL, 0
      , DIREG_DEV, KEY_READ
    );

    if (!hKey) continue;

    size = MAX_PATH;
    BOOL const rv = ::RegQueryValueExA(
        hKey, "PortName"
      , 0, &type
      , (LPBYTE) &port_name
      , &size
    );

    ::RegCloseKey(hKey);
    names.push_back(std::string { port_name });
  }

  SetupDiDestroyDeviceInfoList(hdevinfo);

  return names;
}

void init_arduino(asio::serial_port& port)
{
  DCB dcbSerialParams = { 0 };

  GetCommState(port.native_handle(), &dcbSerialParams);

  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

  SetCommState(port.native_handle(), &dcbSerialParams);

  PurgeComm(port.native_handle(), PURGE_RXCLEAR | PURGE_TXCLEAR);

  Sleep(2000);
}

// never call this function if other asynchronous operations are pending
std::size_t timed_read(
    asio::serial_port& sport
  , asio::mutable_buffer const& buffer
  , std::chrono::milliseconds const& timeout
) {
  bool read_completed = false;
  bool timer_completed = false;
  std::size_t read = 0;

  auto cb = [&](std::error_code const& ec, std::size_t read_bytes) {
    if (!ec) {
      read_completed = true;
      read = read_bytes;
    }
  };

  asio::async_read(sport, buffer, cb);

  asio::steady_timer t(sport.get_io_context(), timeout);

  t.async_wait([&timer_completed](std::error_code const& ec) {
    if (!ec) {
      timer_completed = true;
    }
  });

  sport.get_io_context().reset();

  while (sport.get_io_service().run_one()) {
    if (read_completed) {
      t.cancel();
    } else if (timer_completed) {
      sport.cancel();
    }
  }

  return read;
}

bool is_eee(asio::io_context& ctx, std::string const& port)
{
  try {
    asio::serial_port sport(ctx, port);

    init_arduino(sport);
    asio::write(sport, asio::buffer("e", 1));
    std::string const static expected("EEE\r\n");
    char buffer[6] {};

    auto const s = timed_read(sport, asio::buffer(buffer, 5), 100ms);
    if (s != 5 || expected != buffer) {
      return false;
    }

    return true;
  } catch (std::exception const&) {
    return false;
  }
}

ComPorts probe_filter(asio::io_context& ctx, ComPorts const& in)
{
  ComPorts r;

  for (auto&& port : in) {
    if (is_eee(ctx, port)) {
      r.push_back(port);
    }
  }

  return r;
}

} // ns wsc
} // ns eee
