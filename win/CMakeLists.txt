include_directories(${PROJECT_SOURCE_DIR}/win/include/)
include_directories(${PROJECT_SOURCE_DIR}/win/external/)

find_path(NANA_INCLUDE_DIR nana/gui.hpp)
include_directories(${NANA_INCLUDE_DIR})

find_library(NANA_LIBRARY nana)

if (WIN32)
  find_library(JPEG_LIBRARY jpeg)
  find_library(PNG_LIBRARY libpng16)
  find_library(TURBOJPEG_LIBRARY turbojpeg)
  find_library(ZLIB_LIBRARY zlib)

  set(NANA_LIBRARIES
    ${JPEG_LIBRARY}
    ${PNG_LIBRARY}
    ${TURBOJPEG_LIBRARY}
    ${ZLIB_LIBRARY}
    ${NANA_LIBRARY}
  )
else (WIN32)
  find_package(Threads REQUIRED)
  find_package(X11 REQUIRED)
  find_package(Fontconfig REQUIRED)
  set(
    NANA_LIBRARIES
    ${NANA_LIBRARY}
    ${X11_LIBRARIES}
    ${X11_Xft_LIB}
    ${FONTCONFIG_LIBRARIES}
    Threads::Threads
  )

  include_directories(${X11_INCLUDE_DIR} ${X11_Xft_INCLUDE_DIR})
endif(WIN32)

add_executable(wsc WIN32 src/main.cpp src/gui.cpp src/com.cpp src/worker.cpp)
target_link_libraries(wsc ${NANA_LIBRARIES})
set_target_properties(
  wsc PROPERTIES
  CXX_STANDARD 14
)
target_compile_definitions(
  wsc PRIVATE
  "ASIO_STANDALONE"
  "_WIN32_WINDOWS=0x0601"
  "_WIN32_WINNT=0x0601"
  "WIN32_LEAN_AND_MEAN"
  "STRICT"
  "_WINSOCK_DEPRECATED_NO_WARNINGS"
)
