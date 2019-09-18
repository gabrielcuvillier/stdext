/*
 * Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * */

// Main header
#include <stdext/error>

// STD
#include <cstring>    // for std::strrchr
#include <cstdio>     // for std::printf
#include <string>     // for std::string

template<>
[[nodiscard]] std::string stdext::enum_to_string(stdext::InternalError err)  {
  switch (err) {
    case stdext::InternalError::AssertionFailed: return std::string{ "InternalError::AssertionFailed" };
    default: return std::string{ };
  }
}

void stdext::PRINTERROR(const char * file,
                        int line,
                        const char * func,
                        const char * message)  {
  std::printf("Error at %s:%d: function ‘%s‘: '%s'",
              (std::strrchr(file, '/') ? std::strrchr(file, '/') + 1 : file), // Basename equivalent
              line,
              func,
              message);
}

void stdext::EPICFAIL_RET()  { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_RET_VOID()  { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_RET_INT()  { std::printf(" Aborting function\n"); }

void stdext::EPICFAIL_LOOP()  { std::printf(" Skipping iteration\n"); }

void stdext::EPICFAIL_LOOP_BREAK()  { std::printf(" Aborting loop\n"); }

void stdext::EPICFAIL_NOP()  { std::printf(" Continuing\n"); }
