#include <asio.hpp>

#include "gui.hpp"

int WINAPI WinMain(
    HINSTANCE hInstance
  , HINSTANCE hPrevInstance
  , LPSTR lpCmdLine
  , int nShowCmd
) {
  asio::io_context ctx;

  return eee::wsc::gui(ctx);
}
