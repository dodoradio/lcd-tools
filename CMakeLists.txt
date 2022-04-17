cmake_minimum_required(VERSION 3.21)
project(lcd-tools VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
find_package(CLI11 REQUIRED)
pkg_search_module(LIBHYBRIS REQUIRED hybris-egl-platform)

add_subdirectory(src)

install(TARGETS lcd-tools DESTINATION bin)
install(FILES systemd/lcd-sync-time.service systemd/lcd-sync-time.timer
		DESTINATION /etc/systemd/system)