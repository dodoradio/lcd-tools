add_executable(lcd-tools lcd-tools.cpp lcd-sync-time.cpp)
target_link_libraries(lcd-tools PUBLIC CLI11::CLI11 Mlite5::Mlite5 Qt::Core ${LIBHYBRIS_LIBRARIES})