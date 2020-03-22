# Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
# Licensed under the MIT License.

# Minimum version supported to be able to do "set(CMAKE_CXX_STANDARD 17)"
cmake_minimum_required(VERSION 3.8.2)

project(stdext)

##########################
# Global CMake settings
##########################

# Use C++ 17 standard for the whole project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIREMENT TRUE)

# Determine target platform and compiler

if (WIN32)
  # Windows
  SET(PLATFORM_WINDOWS 1)
  if (MINGW)
    # MinGW
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      # GCC
      SET(COMPILER_GCC 1)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      # Clang
      SET(COMPILER_CLANG 1)
    endif ()
  else ()
    # Visual Studio
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      # MSVC
      SET(COMPILER_MSVC 1)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      # Set both: MSVC+Clang
      SET(COMPILER_MSVC 1)
      SET(COMPILER_CLANG 1)
    endif ()
  endif ()
elseif (EMSCRIPTEN)
  # Emscripten
  SET(PLATFORM_EMSCRIPTEN 1)
  SET(COMPILER_CLANG 1)
elseif (MSYS)
  # MSYS
  SET(PLATFORM_UNIX 1)
  SET(COMPILER_GCC 1)
elseif (CYGWIN)
  # Cygwin
  SET(PLATFORM_UNIX 1)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC
    SET(COMPILER_GCC 1)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang
    SET(COMPILER_CLANG 1)
  endif ()
elseif (UNIX)
  # UNIX generic (Linux for example)
  SET(PLATFORM_UNIX 1)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC
    SET(COMPILER_GCC 1)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang
    SET(COMPILER_CLANG 1)
  endif ()
endif ()

##########################
# Customizable Options
##########################

# Use 'O1' optimization level instead of O0 for Debug (default=OFF)
set(ENABLE_O1 OFF CACHE BOOL "Enable O1 build for Debug")
# Use 'Oz' optimization level instead of Os for MinSizeRel (default=OFF)
set(ENABLE_OZ OFF CACHE BOOL "Enable Oz build for MinSizeRel")
# Use 'O3' optimization level instead of O2 fo Release (default=OFF)
set(ENABLE_O3 OFF CACHE BOOL "Enable O3 build for Release")
# Use LTO for MinSizeRel (default=OFF)
set(ENABLE_LTO OFF CACHE BOOL "Enable LTO")

set(USE_CPP_FILESYSTEM_BACKEND CACHE BOOL "Use std::filesystem backend" ON)

##########################
# Setup compilation flags
##########################

if (USE_CPP_FILESYSTEM_BACKEND)
  add_definitions(-DSTDEXT_USE_CPP_FILESYSTEM_BACKEND)
endif ()

# Enable all warnings
if ((COMPILER_CLANG AND NOT COMPILER_MSVC) OR COMPILER_GCC)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
endif ()

if (COMPILER_MSVC)
  # be sure to have "__cplusplus" macro to correctly report C++ version
  add_compile_options(/Zc:__cplusplus)
  # disable the annoying CRT secure warnings and various C++17 deprecation warnings
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  # increase warning level
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif ()
  if (CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif ()
endif ()

if (PLATFORM_EMSCRIPTEN)
  # disable exception support
  add_compile_options(-fno-exceptions)
  # enable Emscripten STRICT mode for compilation
  add_compile_options(-s STRICT=1)
endif ()

# LTO
if (ENABLE_LTO)
  if (COMPILER_CLANG OR COMPILER_GCC)
    add_compile_options(-flto)
  endif ()
endif ()

if (ENABLE_OZ AND ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel") AND COMPILER_CLANG)
  # Use 'Oz' optimization level (instead of Os)
  string(REGEX MATCH "-Os" IS_Os_CXX "${CMAKE_CXX_FLAGS_MINSIZEREL}")
  if (IS_Os_CXX)
    string(REGEX REPLACE "-Os" "-Oz" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
  else ()
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Oz")
  endif ()

  string(REGEX MATCH "-Os" IS_Os_C "${CMAKE_C_FLAGS_MINSIZEREL}")
  if (IS_Os_C)
    string(REGEX REPLACE "-Os" "-Oz" CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
  else ()
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Oz")
  endif ()
endif ()

# Properly setup RelWithDebInfo
if (("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo") AND (COMPILER_CLANG OR COMPILER_GCC))
  string(REGEX MATCH "-g" IS_g_CXX "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  if (NOT IS_g_CXX)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")
  endif ()

  string(REGEX MATCH "-g" IS_g_C "${CMAKE_CC_FLAGS_RELWITHDEBINFO}")
  if (NOT IS_g_C)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g")
  endif ()
endif ()

if (ENABLE_O3 AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release") AND (COMPILER_CLANG OR COMPILER_GCC))
  # Use 'O3' optimization level (instead of O2)
  string(REGEX MATCH "-O2" IS_O2_CXX "${CMAKE_CXX_FLAGS_RELEASE}")
  if (IS_O2_CXX)
    string(REGEX REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  else ()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  endif ()

  string(REGEX MATCH "-O2" IS_O2_C "${CMAKE_C_FLAGS_RELEASE}")
  if (IS_O2_C)
    string(REGEX REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  else ()
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
  endif ()
endif ()

if (ENABLE_O1 AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") AND (COMPILER_CLANG OR COMPILER_GCC))
  # Use 'O1' optimization level (instead of O0)
  string(REGEX MATCH "-O0" IS_O0_CXX "${CMAKE_CXX_FLAGS_DEBUG}")
  if (IS_O0_CXX)
    string(REGEX REPLACE "-O0" "-O1" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  else ()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")
  endif ()

  string(REGEX MATCH "-O0" IS_O0_C "${CMAKE_C_FLAGS_DEBUG}")
  if (IS_O0_C)
    string(REGEX REPLACE "-O0" "-O1" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  else ()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O1")
  endif ()
endif ()

############
# libstdext
###########"
add_library(stdext STATIC
    lib/stdext_compiler_id.cpp
    lib/call_release_func.cpp
    lib/exception_absorb.cpp
    lib/uuid.cpp
    lib/fs.cpp
    lib/error.cpp)

# Includes
target_include_directories(stdext
    PUBLIC include
    PRIVATE include_private)

if (PLATFORM_EMSCRIPTEN)
  # uuid
  target_link_libraries(stdext PUBLIC uuid.js)
elseif (PLATFORM_UNIX)
  # uuid
  target_link_libraries(stdext PUBLIC uuid)
elseif (PLATFORM_WINDOWS)
  # uuid
  target_link_libraries(stdext PUBLIC rpcrt4)
  # filesystem-related
  # NB: not on MinGW, as UNIX compatibility layer is used
  if (NOT MINGW AND NOT USE_CPP_FILESYSTEM_BACKEND)
    target_link_libraries(stdext PUBLIC shlwapi)
  endif ()
endif ()


############
# test/main
###########"
add_executable(test_main test/main/main.cpp)

# Includes
target_include_directories(test_main
    PRIVATE $<TARGET_PROPERTY:stdext,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(test_main stdext)

if (PLATFORM_UNIX OR MINGW)
  target_link_libraries(test_main stdc++ stdc++fs m)
endif ()