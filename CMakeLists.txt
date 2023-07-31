cmake_minimum_required(VERSION 3.16)
project(ParAlgFramework)

# set default for MARCH
if(NOT MARCH)
  set(MARCH native)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSTIONS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSTIONS ON)

find_package(TBB)
find_package(OpenMP)

add_executable(ParAlgFramework main.cpp)
target_compile_options(ParAlgFramework PUBLIC $<$<CONFIG:Release>:-march=native> -Wpedantic -Wall -Wextra)

if(OpenMP_CXX_FOUND)
  target_compile_definitions(ParAlgFramework PUBLIC HAS_OMP)
  target_link_libraries(ParAlgFramework PUBLIC OpenMP::OpenMP_CXX)
endif()

if(TBB_FOUND)
  target_compile_definitions(ParAlgFramework PUBLIC HAS_TBB)
  target_link_libraries(ParAlgFramework PUBLIC tbb)

  if(TBB_INTERFACE_VERSION LESS 11004)
    message("Using TBB preview features")
    target_compile_definitions(TBB_PREVIEW_GLOBAL_CONTROL)
  endif()
endif()
