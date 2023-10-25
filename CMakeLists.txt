cmake_minimum_required(VERSION 3.10)

project(HFTSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add path to pybind11's CMake configuration
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/python3.11/site-packages/pybind11/share/cmake/pybind11")


# Find pybind11
find_package(pybind11 REQUIRED)

# Include directories
include_directories("/usr/local/Cellar/ta-lib/0.4.0/include")
include_directories(${CMAKE_SOURCE_DIR}/src)

# Link directories
link_directories("/usr/local/Cellar/ta-lib/0.4.0/lib")

# Executable
add_executable(HFTSystem src/main.cpp src/MovingAverageStrategy.cpp src/Portfolio.cpp src/StockData.cpp src/Backtester.cpp)

# Explicitly link the library
target_link_libraries(HFTSystem "/usr/local/Cellar/ta-lib/0.4.0/lib/libta_lib.0.dylib" pybind11::embed)

# Set RPATH properties
set_target_properties(HFTSystem PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(HFTSystem PROPERTIES INSTALL_RPATH "/usr/local/Cellar/ta-lib/0.4.0/lib/")


