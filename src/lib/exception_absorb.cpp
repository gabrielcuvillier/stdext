// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/exception_absorb>

// std
#include <exception>
#include <iostream>
#include <typeinfo>

stdext::_ExceptionAbsorber::_ExceptionAbsorber( std::exception const& err )
{
  std::cerr << "Exception thrown: " << typeid( err ).name() << ": \"" << err.what() << "\""
            << std::endl;
}

stdext::_ExceptionAbsorber::~_ExceptionAbsorber()
{
  std::terminate();  // Ideally, std::set_terminate should be set by the main application
}
