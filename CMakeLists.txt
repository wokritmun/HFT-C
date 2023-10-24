cmake_minimum_required(VERSION 3.10)

project(HFTSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("/usr/local/Cellar/ta-lib/0.4.0/include")
link_directories("/usr/local/Cellar/ta-lib/0.4.0/lib")

add_executable(HFTSystem main.cpp)

target_link_libraries(HFTSystem ta)
