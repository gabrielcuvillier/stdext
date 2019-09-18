/*
 * Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * */

// Main header
#include <stdext/call_release_func>

stdext::call_release_func::call_release_func(void(*ipFunc)()) noexcept : _pFunc(ipFunc) {
}

stdext::call_release_func::call_release_func(call_release_func &&iMoveFunc) noexcept : _pFunc(iMoveFunc._pFunc) {
  iMoveFunc._pFunc = nullptr;
}

stdext::call_release_func::~call_release_func() {
  if (_pFunc) {
    _pFunc();
    _pFunc = nullptr;
  }
}

stdext::call_release_func &stdext::call_release_func::operator=(call_release_func &&iMoveFunc) noexcept {
  _pFunc = iMoveFunc._pFunc;
  iMoveFunc._pFunc = nullptr;
  return *this;
}
