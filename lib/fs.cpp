// Copyright (c) 2019 - Gabriel Cuvillier, Continuation Labs (www.continuation-labs.com)
// Licensed under the MIT License.

// Main header
// #include <stdext/fs>

// std
#include <cstring>  // std::strcpy
#include <string>   // std::string

#if defined( STDEXT_USE_CPP_FILESYSTEM_BACKEND )
#if __has_include( <filesystem> )
//#pragma message("<filesystem>")
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include( <experimental/filesystem> )
//#pragma message("<experimental/filesystem>")
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "std::filesystem not implemented for current platform"
#endif

namespace stdext
{
std::string get_current_directory() { return fs::current_path().string(); }

void change_current_directory( std::string const& path ) { fs::current_path( path ); }

std::string get_file_name( std::string const& path ) { return fs::path( path ).filename().string(); }

std::string get_directory_name( std::string const& path )
{
  return fs::path( path ).remove_filename().filename().string();
}
}  // namespace stdext

#else

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
std::string safe_apply_to_str( std::string const& str )
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

int change_current_directory( std::string const& path )
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

std::string get_file_name( std::string const& path )
{
#if defined( _WIN32 ) && !defined( __MINGW32__ )
  return std::string( ::PathFindFileNameA( path.c_str() ) );
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  return safe_apply_to_str< ::basename>( path );
#else
#error "std::filesystem not implemented for current platform"
#endif
}

std::string get_directory_name( std::string const& path )
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
    return std::string();
  }
#elif defined( __linux__ ) || defined( __gnu_linux__ ) || defined( __MSYS__ ) || defined( __CYGWIN__ ) || \
    defined( __MINGW32__ ) || defined( __EMSCRIPTEN__ ) || defined( __unix__ )
  return safe_apply_to_str< ::dirname>( path );
#else
#error "std::filesystem not implemented for current platform"
#endif
}
}  // namespace stdext
#endif