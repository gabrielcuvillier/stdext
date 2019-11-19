// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/exception_absorb>

// std
#include <cstdio>     // std::fprintf, stdout, stderr
#include <exception>  // std::exception
#include <typeinfo>   // typeid

stdext::_ExceptionAbsorber::_ExceptionAbsorber( std::exception const& err )
{
  std::fprintf( stderr, "Exception thrown: %s: \"%s\"\n", typeid( err ).name(), err.what() );
}

stdext::_ExceptionAbsorber::~_ExceptionAbsorber()
{
  std::terminate();  // Ideally, std::set_terminate should be set by the main application
}
