//
// Created by Gabriel on 22/03/2020.
//
#include <stdext/error>

std::error_condition test_func() {
  return std::error_condition();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
  stdext::install_unhandled_exception_handler();

  ASSERT_RET_ONE(test_func())

  return 0;
}
