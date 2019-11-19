// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/error>

// STD
#include <cstdio>     // std::fprintf, stdout, stderr
#include <cstdlib>    // std::abort
#include <cstring>    // std::strcpy, std::strlen
#include <exception>  // std::set_terminate
#if !defined( __MINGW32__ )
#include <filesystem>  // std::filesystem
#else
// std::filesystem::path is buggy for now on MinGW, so use regular POSIX libgen functions
#include <libgen.h>
#endif

void stdext::install_unhandled_exception_handler()
{
  std::set_terminate( []() {
    std::fprintf( stderr, "Terminate handler: aborting the program." );
    std::abort();
  } );
}

template<>
stdext_NODISCARD const char* stdext::enum_to_string( stdext::InternalError err )
{
  switch ( err ) {
    case stdext::InternalError::AssertionFailed: return "InternalError::AssertionFailed";
  }
}

void stdext::PRINTERROR( const char* const file, const int line, const char* const func, const char* const message )
{
#if !defined( __MINGW32__ )
  std::fprintf( stdout, "Failure at %s:%d: function ‘%s‘: '%s'", std::filesystem::path( file ).filename().c_str(), line,
                func, message );
#else
  char* str = nullptr;
  if ( file ) {
    str = new char[std::strlen( file ) + 1];
    str[std::strlen( file )] = 0;
    std::strcpy( str, file );
  } else {
    str = new char[1];
    str[0] = 0;
  }
  std::fprintf( stdout, "Failure at %s:%d: function ‘%s‘: '%s'", basename( str ), line, func, message );
  delete[] str;
#endif
}

namespace
{
const char* const abort_msg = " Aborting function\n";
const char* const skipping_iteration_msg = " Skipping iteration\n";
const char* const abort_loop_msg = " Aborting loop\n";
const char* const continue_msg = " Continuing\n";
}  // namespace

void stdext::EPICFAIL_RET() { std::fprintf( stdout, abort_msg ); }

void stdext::EPICFAIL_RET_VOID() { std::fprintf( stdout, abort_msg ); }

void stdext::EPICFAIL_RET_INT() { std::fprintf( stdout, abort_msg ); }

void stdext::EPICFAIL_LOOP() { std::fprintf( stdout, skipping_iteration_msg ); }

void stdext::EPICFAIL_LOOP_BREAK() { std::printf( abort_loop_msg ); }

void stdext::EPICFAIL_NOP() { std::printf( continue_msg ); }
