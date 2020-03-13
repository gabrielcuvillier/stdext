// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/error>

// std
#include <cstdio>     // std::fprintf, stderr
#include <cstdlib>    // std::abort
#include <exception>  // std::set_terminate
#include <string>     // std::string

// stdext
#include <stdext/fs>

void stdext::install_unhandled_exception_handler()
{
  std::set_terminate( []() {
    std::fprintf( stderr, "Terminate handler called: aborting the program.\n" );
    std::abort();
  } );
}

template<>
const char* stdext::enum_to_string( stdext::InternalError err )
{
  switch ( err ) {
    case stdext::InternalError::AssertionFailed: return "InternalError::AssertionFailed";
  }
  return "InternalError::<unknown>";
}

void stdext::PRINTERROR( const char* file, int line, int col, const char* func, const char* message )
{
  std::fprintf( stderr, "Failure in %s, line %d:%d: function '%s': '%s'\n",
                stdext::get_file_name( std::string( file ) ).c_str(), line, col, func, message );
}

namespace
{
const char* const abort_msg = " Aborting function\n";
const char* const abort_prog = " Aborting program\n";
const char* const skipping_iteration_msg = " Skipping iteration\n";
const char* const abort_loop_msg = " Aborting loop\n";
const char* const continue_msg = " Continuing\n";
}  // namespace

void stdext::EPICFAIL_RET() { std::fprintf( stderr, abort_msg ); }

void stdext::EPICFAIL_CRASH() { std::fprintf( stderr, abort_prog ); }

void stdext::EPICFAIL_RET_VOID() { std::fprintf( stderr, abort_msg ); }

void stdext::EPICFAIL_RET_INT() { std::fprintf( stderr, abort_msg ); }

void stdext::EPICFAIL_LOOP() { std::fprintf( stderr, skipping_iteration_msg ); }

void stdext::EPICFAIL_LOOP_BREAK() { std::fprintf( stderr, abort_loop_msg ); }

void stdext::EPICFAIL_NOP() { std::fprintf( stderr, continue_msg ); }
