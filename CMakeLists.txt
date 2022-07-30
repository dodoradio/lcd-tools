cmake_minimum_required(VERSION 3.21)
project(lcd-tools VERSION 0.1)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
find_package(CLI11 REQUIRED)
find_package(Mlite5 MODULE REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
pkg_check_modules(LIBHYBRIS REQUIRED hybris-egl-platform libandroid-properties)

add_subdirectory(src)

install(TARGETS lcd-tools DESTINATION bin)
install(FILES systemd/lcd-sync-time.service systemd/lcd-sync-time.timer
		DESTINATION /usr/lib/systemd/user)