cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

project(wplotlib VERSION 0.1.0 LANGUAGES C CXX)

add_library(wplotlib STATIC source/wplotlib/DataSet.cpp source/wplotlib/PlotWidget.cpp)