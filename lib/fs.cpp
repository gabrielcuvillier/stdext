// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
#include <stdext/fs>

// std
#include <cstring>  // std::strcpy
#include <string>   // std::string

// MSVC
#if defined( _WIN32 ) && !defined( __MINGW32__ )  // Let's use regular unix-style calls for MinGW
#define USE_STRICT_CONST
#include <Shlwapi.h>  // PathFindFileNameA, PathFindDirNameA
#include <direct.h>   // _chdir, _get_cwd
// LINUX, GNU/LINUX, MSYS, MINGW, CYGWIN, EMSCRIPTEN
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
#include <libgen.h>  // basename, dirname
#include <unistd.h>  // chdir, getcwd

#include <climits>  // PATH_MAX
#else
#error "std::filesystem not implemented for current platform"
#endif

namespace
{
#if defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
template<char* ( *F )( char* )>
std::string safe_apply_to_str( std::string str )
{
  auto psz_tmp = new char[str.size() + 1];
  psz_tmp[str.size()] = 0;
  std::strcpy( psz_tmp, str.c_str() );
  std::string result_str = F( psz_tmp );
  delete[] psz_tmp;
  return result_str;
}
#endif
}  // namespace

namespace stdext
{
std::string get_current_directory()
{
#if defined( _WIN32 ) && !defined( __MINGW32__ )
  auto buf = new char[MAX_PATH];
  buf = ::_getcwd( buf, MAX_PATH );
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  auto buf = new char[PATH_MAX];
  buf = ::getcwd( buf, PATH_MAX );
#else
#error "std::filesystem not implemented for current platform"
#endif
  std::string result_str = buf;
  delete[] buf;
  return result_str;
}

int change_current_directory( std::string path )
{
#if defined( _WIN32 ) && !defined( __MINGW32__ )
  return ::_chdir( path.c_str() );
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  return ::chdir( path.c_str() );
#else
#error "std::filesystem not implemented for current platform"
#endif
}

std::string get_file_name( std::string path )
{
#if defined( _WIN32 ) && !defined( __MINGW32__ )
  return std::string( ::PathFindFileNameA( path.c_str() ) );
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  return safe_apply_to_str< ::basename>( std::move( path ) );
#else
#error "std::filesystem not implemented for current platform"
#endif
}

std::string get_directory_name( std::string path )
{
#if defined( _WIN32 ) && !defined( __MINGW32__ )
  auto psz_tmp = new char[path.size() + 1];
  psz_tmp[path.size()] = 0;
  std::strcpy( psz_tmp, path.c_str() );
  if ( ::PathRemoveFileSpecA( psz_tmp ) ) {
    std::string result_str = psz_tmp;
    delete[] psz_tmp;
    return result_str;
  } else {
    delete[] psz_tmp;
    return std::string( nullptr );
  }
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  return safe_apply_to_str< ::dirname>( path );
#else
#error "std::filesystem not implemented for current platform"
#endif
}
}  // namespace stdext
