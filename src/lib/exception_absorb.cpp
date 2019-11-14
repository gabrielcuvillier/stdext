// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/exception_absorb>

// std
#include <exception>
#include <iostream>
#include <string>
#include <typeinfo>

stdext::_ExceptionAbsorber::_ExceptionAbsorber( std::exception const& err ) noexcept
    : type_( typeid( err ).name() ), message_( err.what() )
{
}

stdext::_ExceptionAbsorber::~_ExceptionAbsorber() noexcept
{
  std::cerr << type_ << ": " << message_ << std::endl;
  std::terminate();  // Ideally, std::set_terminate should be set by the main application
};
