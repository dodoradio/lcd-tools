add_executable(lcd-tools lcd-tools.cpp lcd-sync-time.cpp)
target_link_libraries(lcd-tools PUBLIC CLI11::CLI11 ${LIBHYBRIS_LIBRARIES})