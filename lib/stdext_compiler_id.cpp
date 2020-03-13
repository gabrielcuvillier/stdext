// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

#if 0   // Enable to check the compiler predefined macros

#include <climits>

// OS
#if defined( _WIN32 )
#pragma message( "_WIN32" )
#endif
#if defined( _WIN64 )
#pragma message( "_WIN64" )
#endif
#if defined( __unix__ )
#pragma message( "__unix__" )
#endif
#if defined( __linux__ )
#pragma message( "__linux__" )
#endif
#if defined( __gnu_linux__ )
#pragma message( "__gnu_linux__" )
#endif

// POSIX compatibility
#if defined( __unix__ ) || defined( __MINGW32__ )
#include <unistd.h>
#if defined( _POSIX_VERSION )
#pragma message( "_POSIX_VERSION" )
#endif
#if defined( _XOPEN_VERSION )
#pragma message( "_XOPEN_VERSION" )
#endif
#if defined( __LSB_VERSION__ )
#pragma message( "__LSB_VERSION__" )
#endif
#endif

// Compiler Environments
#if defined( __MINGW32__ )
#pragma message( "__MINGW32__" )
#endif
#if defined( __MINGW64__ )
#pragma message( "__MINGW64__" )
#endif
#if defined( __MSYS__ )
#pragma message( "__MSYS__" )
#endif
#if defined( __CYGWIN__ )
#pragma message( "__CYGWIN__" )
#endif
#if defined( __EMSCRIPTEN__ )
#pragma message( "__EMSCRIPTEN__" )
#endif

// Compilers
#if defined( _MSC_VER )
#pragma message( "_MSC_VER" )
#endif
#if defined( __clang__ )
#pragma message( "__clang__" )
#endif
#if defined( __GNUC__ )
#pragma message( "__GNUC__" )
#endif

// Libs
#if defined( __GLIBC__ )
#pragma message( "__GLIBC__" )
#endif
#if defined( __STDC__ )
#pragma message( "__STDC__" )
#endif

#endif