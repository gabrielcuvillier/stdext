// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/error>

// STD
#include <cstdio>     // std::fprintf, stdout, stderr
#include <cstdlib>    // std::abort
#include <cstring>    // std::strcpy, std::strlen
#include <exception>  // std::set_terminate
#if !defined( _WIN32 )
#include <libgen.h>  // basename
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
  char* file_copy = nullptr;
  if ( file ) {
    const auto sz = std::strlen( file );
    file_copy = new char[sz + 1];
    file_copy[sz] = 0;
    std::strcpy( file_copy, file );
  }
  std::fprintf( stdout, "Failure at %s:%d: function ‘%s‘: '%s'",
#if !defined( _WIN32 )
                basename( file_copy ),
#else
                file_copy,
#endif
                line, func, message );
  delete[] file_copy;
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
