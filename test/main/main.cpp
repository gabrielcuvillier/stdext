//
// Created by Gabriel on 22/03/2020.
//
#include <iostream>
#include <stdext/error>
#include <stdext/fs>

std::error_condition test_func() { return std::error_condition(); }

int main( int argc, const char** argv ) noexcept
{
  stdext::install_unhandled_exception_handler();

  ASSERT_RET_ONE( test_func() )

  std::cout << stdext::get_current_directory() << std::endl;
  if (argc > 0) {
    std::cout << stdext::get_file_name( argv[0] ) << std::endl;
    std::cout << stdext::get_directory_name( argv[0] ) << std::endl;
  }

  return 0;
}
