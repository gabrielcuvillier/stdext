// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/call_release_func>

stdext::call_release_func::call_release_func( void ( *ipFunc )() ) : _pFunc( ipFunc ) {}

stdext::call_release_func::call_release_func( call_release_func&& iMoveFunc ) : _pFunc( iMoveFunc._pFunc )
{
  iMoveFunc._pFunc = nullptr;
}

stdext::call_release_func::~call_release_func()
{
  if ( _pFunc ) {
    _pFunc();
    _pFunc = nullptr;
  }
}

stdext::call_release_func& stdext::call_release_func::operator=( call_release_func&& iMoveFunc )
{
  _pFunc = iMoveFunc._pFunc;
  iMoveFunc._pFunc = nullptr;
  return *this;
}