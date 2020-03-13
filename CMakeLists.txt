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
      # MSVC+Clang
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

# Use 'O1' optimization level instead of O2 for RelWithDebInfo (default=ON)
set(ENABLE_O1 ON CACHE BOOL "Enable O1 build for RelWithDebInfo")
# Use LTO for MinSizeRel (default=OFF)
set(ENABLE_LTO OFF CACHE BOOL "Enable LTO")
# Use 'Oz' optimization level instead of Os for MinSizeRel (default=OFF)
set(ENABLE_OZ OFF CACHE BOOL "Enable Oz build for MinSizeRel")
# Use 'O3' optimization level instead of O2 fo Release (default=OFF)
set(ENABLE_O3 OFF CACHE BOOL "Enable O3 build for Release")

##########################
# Setup compilation flags
##########################

# Enable all warnings
if (COMPILER_CLANG OR COMPILER_GCC)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
endif ()

if (COMPILER_MSVC)
  add_compile_options(/Zc:__cplusplus)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  if (COMPILER_CLANG)
    add_compile_options(-Wno-c++98-compat)
    add_compile_options(-Wno-global-constructors)
  endif()
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

# Oz and MinSizeRel
if (ENABLE_OZ AND COMPILER_CLANG)
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

  string(REGEX MATCH "-Os" IS_Os_EXE "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
  if (IS_Os_EXE)
    string(REGEX REPLACE "-Os" "-Oz" CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
  else ()
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -Oz")
  endif ()
endif ()

# O3 and Release
if (ENABLE_O3 AND (COMPILER_CLANG OR COMPILER_GCC))
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

  string(REGEX MATCH "-O2" IS_O2_EXE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
  if (IS_O2_EXE)
    string(REGEX REPLACE "-O2" "-O3" CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
  else ()
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O3")
  endif ()
endif ()

# O1 and RelWithDebInfo
if (ENABLE_O1 AND (COMPILER_CLANG OR COMPILER_GCC))
  string(REGEX MATCH "-O2" IS_O2_CXX "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  if (IS_O2_CXX)
    string(REGEX REPLACE "-O2" "-O1" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  else ()
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O1")
  endif ()

  string(REGEX MATCH "-O2" IS_O2_C "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
  if (IS_O2_C)
    string(REGEX REPLACE "-O2" "-O1" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
  else ()
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O1")
  endif ()

  string(REGEX MATCH "-O2" IS_O2_EXE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
  if (IS_O2_EXE)
    string(REGEX REPLACE "-O2" "-O1" CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
  else ()
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} -O1")
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
        PRIVATE include/stdext
        PRIVATE include_private)

if (PLATFORM_EMSCRIPTEN)
  target_link_libraries(stdext PUBLIC uuid.js)    # uuid functions
elseif (PLATFORM_UNIX)
  target_link_libraries(stdext PUBLIC uuid)       # uuid functions
elseif (PLATFORM_WINDOWS)
  target_link_libraries(stdext PUBLIC rpcrt4)     # uuid functions
  if (COMPILER_MSVC)
    target_link_libraries(stdext PUBLIC shlwapi)  # filesystem-related functions
  endif ()
endif ()
