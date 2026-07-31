cmake_minimum_required(VERSION 3.22)
project(mira_darray_lib LANGUAGES C)

set(MIRA_DARRAY_VERSION "1.0.0-beta.2")

add_library(mira_darray INTERFACE)

target_include_directories(mira_darray INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)