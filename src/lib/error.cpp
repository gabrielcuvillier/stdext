// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/error>

// STD
#include <cstdlib>    // std::abort
#include <cstdio>     // std::printf
#include <iostream>   // std::cerr, std::endl
#include <string>     // std::string
#include <exception>  // std::set_terminate
#if !defined(__GNUC__)||defined(__clang__)
#include <filesystem> // std::filesystem
#endif

void stdext::install_unhandled_exception_handler() {
  std::set_terminate([]() {
    std::cerr << "[stdext] Unhandled exception. Aborting the program." << std::endl;
    std::abort();
  });
}

template<>
stdext_NODISCARD std::string stdext::enum_to_string(stdext::InternalError err) {
  switch (err) {
    case stdext::InternalError::AssertionFailed: return std::string{"InternalError::AssertionFailed"};
    default: return std::string{};
  }
}

void stdext::PRINTERROR(const char *file,
                        int line,
                        const char *func,
                        const char *message) {
  std::printf("Failure at %s:%d: function ‘%s‘: '%s'",
#if !defined(__GNUC__)||defined(__clang__)
              std::filesystem::path(file).filename().c_str(),
#else
              file,
#endif
              line,
              func,
              message);
}

void stdext::EPICFAIL_RET() { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_RET_VOID() { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_RET_INT() { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_LOOP() { std::printf(" Skipping iteration\n"); }

void stdext::EPICFAIL_LOOP_BREAK() { std::printf(" Aborting loop\n"); }

void stdext::EPICFAIL_NOP() { std::printf(" Continuing\n"); }
