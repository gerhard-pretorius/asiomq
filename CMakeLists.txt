#[[
CC=clang CXX=clang cmake -G "Unix Makefiles" \
-DCMAKE_PREFIX_PATH=$HOME/opt \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$HOME/install \
-DCMAKE_CXX_FLAGS="-std=c++17 -stdlib=libc++ -v" \
-DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++ -lc++ -lc++abi" \
$HOME/github/asiomq
]]


#----------------------------------------------------------
cmake_minimum_required (VERSION 3.10)
project(asiomq)

include(version.cmake)


#----------------------------------------------------------
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

#----------------------------------------------------------
# libraries/includes
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE on)

find_package(Protobuf)
message("Using Protobuf ${Protobuf_VERSION} : ${Protobuf_PROTOC_EXECUTABLE}")

include_directories(${Protobuf_INCLUDE_DIRS})

#----------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")


#----------------------------------------------------------
# install paths
set(HDR_DIR include/asiomq)
set(LIB_DIR lib/asiomq)


#----------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(package)
include(protobuf)

#----------------------------------------------------------
include_directories(lib)
add_subdirectory(lib)
add_subdirectory(example)

#----------------------------------------------------------

