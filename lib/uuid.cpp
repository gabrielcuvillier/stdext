// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
// #include <stdext/uuid>

// std
#include <string>  // std::string

#if defined( _WIN32 )  // also includes MinGW, as it does not have <uuid/uuid.h>
// MSVC, MINGW
#include <rpc.h>
#include <rpcdce.h>
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __EMSCRIPTEN__ )
// LINUX, GNU/LINUX, MSYS, CYGWIN, EMSCRIPTEN
#include <uuid/uuid.h>
#else
// Missing/To Check: BSD variants, APPLE, iOS, Android
#error "UUID generation not implemented for current platform"
#endif

namespace stdext
{
std::string gen_uuid_string()
{
#if defined( _WIN32 )  // also includes MinGW
  UUID uuid;
  UuidCreate( &uuid );
  RPC_CSTR psz_uc_uuid = nullptr;
  UuidToStringA( &uuid, &psz_uc_uuid );
  // For std::string compatibility, we need a char* and not a unsigned char* (RPC_CSTR)
  std::string str_uuid = reinterpret_cast<const char*>( psz_uc_uuid );
  RpcStringFreeA( &psz_uc_uuid );
  return str_uuid;
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __EMSCRIPTEN__ )
  uuid_t uuid;
  uuid_generate( uuid );
  static const auto UUID_STRING_LENGTH = 36;
  char psz_uuid[UUID_STRING_LENGTH + 1] = {0};
  uuid_unparse( uuid, psz_uuid );
  return psz_uuid;
#else
#error "UUID generation not implemented for current platform"
#endif
}
}  // namespace stdext
