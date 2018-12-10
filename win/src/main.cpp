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
  return eee::wsc::gui();
}
