cmake_minimum_required(VERSION 3.10)
project(kp-backup)

set(CMAKE_CXX_STANDARD 17)

add_executable(kp-backup src/main.cpp)
install(TARGETS kp-backup DESTINATION bin)
install(FILES config/kp-backup.conf DESTINATION /etc)
