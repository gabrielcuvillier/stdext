// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/uuid>

#include <string>

#if defined( _MSC_VER ) || defined( __MINGW32__ )
#include <rpc.h>
#include <rpcdce.h>
#elif defined( __gnu_linux__ ) || defined( __EMSCRIPTEN__ )
#include <uuid/uuid.h>
#endif

namespace
{
static const short UUID_STRING_LENGTH = 36;
}

std::string stdext::gen_uuid_string()
{
#if defined( _MSC_VER ) || defined( __MINGW32__ )
  UUID uuid;
  UuidCreate( &uuid );
  char* str = nullptr;
  UuidToStringA( &uuid, (RPC_CSTR*)&str );
  std::string uuid_str = str;
  RpcStringFreeA( (RPC_CSTR*)&str );
  return std::move( uuid_str );
#elif defined( __gnu_linux__ ) || defined( __EMSCRIPTEN__ )
  uuid_t id;
  uuid_generate( id );
  char str[UUID_STRING_LENGTH + 1];
  str[UUID_STRING_LENGTH] = 0;
  uuid_unparse( id, str );
  return std::string( str );
#else
#error "UUID generation not implemented for current platform"
#endif
}
