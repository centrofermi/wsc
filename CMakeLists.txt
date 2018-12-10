project(WSC CXX)
cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory("win")

set(CPACK_PACKAGE_NAME "WSC")
set(CPACK_PACKAGE_VENDOR "EEE")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EEE Weather Station Control program")
set(CPACK_CREATE_DESKTOP_LINKS ON)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

install(FILES "${CPACK_RESOURCE_FILE_LICENSE}" DESTINATION .)

if(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_MENU_LINKS "https://github.com/centrofermi/wsc" "Website")
  set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\\\$SMPROGRAMS\\\\\$STARTMENU_FOLDER\\\\Weather Station Control.lnk' '\\\$INSTDIR\\\\wsc.exe'")
  set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
  set(CPACK_NSIS_MUI_FINISHPAGE_RUN "wsc.exe")
else(WIN32)
  set(CPACK_GENERATOR STGZ)
endif(WIN32)

include(CPack)
