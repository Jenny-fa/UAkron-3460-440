cmake_minimum_required(VERSION 3.0)

# Set CMake module path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckCXXSymbolExists)
include(CheckIncludeFileCXX)

# Add a project.
project(simple-calc VERSION 0.1 LANGUAGES CXX)

# Set variables.
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_TARNAME "${PACKAGE_NAME}")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "jy38@zips.uakron.edu")
set(PACKAGE_URL "https://github.com/Jenny-fa/UAkron-3460-440")

# Add .ipp and .IPP to list of ignored file extensions for C++.
list(APPEND CMAKE_CXX_IGNORE_EXTENSIONS "ipp" "IPP")
list(REMOVE_DUPLICATES CMAKE_CXX_IGNORE_EXTENSIONS)

# Add options.
option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(MOAR_DIGITS "Allow calculator expressions containing multi-digit (instead of single-digit) integer literals." OFF)

# Enable testing.
enable_testing()

# Run platform checks.
check_cxx_compiler_flag(-std=c++14 CXX_COMPILER_HAS_STDCXX14_FLAG)
if(CXX_COMPILER_HAS_STDCXX14_FLAG)
	set(CMAKE_REQUIRED_FLAGS -std=c++14)
endif()
check_include_file_cxx(unistd.h HAVE_UNISTD_H)
check_include_file_cxx(experimental/string_view HAVE_EXPERIMENTAL_STRING_VIEW)
if(NOT HAVE_EXPERIMENTAL_STRING_VIEW)
	message(FATAL_ERROR "${PROJECT_NAME} requires the C++ standard library header <experimental/string_view>.")
endif()
check_cxx_symbol_exists("std::isblank<char>(char, std::locale)" locale HAVE_STD_ISBLANK)

set(TEST_NUMERIC_CONVERSIONS_SOURCE "
#if USE_STD_NAMESPACE
#  include <cstdlib>
#  include <cwchar>
#  define STD std
#else
#  include <stdlib.h>
#  include <wchar.h>
#  define STD
#endif

int main() {
  const char* cstr = \"42\";
  const wchar_t* wcstr = L\"42\";
  long longv = STD::strtol(cstr, nullptr, 10), wlongv = STD::wcstol(wcstr, nullptr, 10);
  long long llongv = STD::strtoll(cstr, nullptr, 10), wllongv = STD::wcstoll(wcstr, nullptr, 10);
  return 0;
}
")

set(CMAKE_REQUIRED_DEFINITIONS_SAVE ${CMAKE_REQUIRED_DEFINITIONS})
set(CMAKE_REQUIRED_DEFINITIONS -DUSE_STD_NAMESPACE=0)
check_cxx_source_compiles("${TEST_NUMERIC_CONVERSIONS_SOURCE}" HAVE_NUMERIC_CONVERSIONS)
if(NOT HAVE_NUMERIC_CONVERSIONS)
  message(FATAL_ERROR "${PROJECT_NAME} requires the C library numeric conversion functions strtol(), strtoll(), wcstol(), and wcstoll().")
endif()
set(CMAKE_REQUIRED_DEFINITIONS -DUSE_STD_NAMESPACE=1)
check_cxx_source_compiles("${TEST_NUMERIC_CONVERSIONS_SOURCE}" HAVE_STD_NUMERIC_CONVERSIONS)
set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS_SAVE})

check_cxx_source_compiles("
#include <string>

int main() {
  std::string str(\"42\");
  std::wstring wstr(L\"42\");
  int intv = std::stoi(str), wintv = std::stoi(wstr);
  long long llongv = std::stoll(str), wllongv = std::stoll(wstr);
  return 0;
}
" HAVE_STD_STRING_NUMERIC_CONVERSIONS)

# Set compiler and linker flags.
if(CXX_COMPILER_HAS_STDCXX14_FLAG)
	add_compile_options(-std=c++14)
endif()

# Add the library and executable targets.
add_library(libcalc
	ast.cpp
	cli.cpp
	lexer.cpp
	parse_error.cpp
	parser.cpp
	script.cpp
	symbol_traits.cpp
	token.cpp)
set_target_properties(libcalc PROPERTIES OUTPUT_NAME calc)
if(CXX_COMPILER_HAS_STDCXX14_FLAG)
	target_compile_options(libcalc PUBLIC -std=c++14)
endif()
target_include_directories(libcalc PUBLIC "$<BUILD_INTERFACE:${simple-calc_SOURCE_DIR};${simple-calc_BINARY_DIR}>")

add_executable(calc calc.cpp)
target_link_libraries(calc libcalc)

# Generate the configuration header.
configure_file(config.hpp.in config.hpp)

# Add 'check' target.
if(NOT TARGET check)
	add_custom_target(check
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test)
endif()

# Add subdirectories.
add_subdirectory(test EXCLUDE_FROM_ALL)
