#include <asio.hpp>

#include "compat.hpp"
#include "gui.hpp"

#ifdef WSC_ON_WIN
int WINAPI WinMain(
    HINSTANCE hInstance
  , HINSTANCE hPrevInstance
  , LPSTR lpCmdLine
  , int nShowCmd
#else // WSC_ON_WIN
int main(
#endif // WSC_ON_WIN
) {
  asio::io_context ctx;

  return eee::wsc::gui(ctx);
}
