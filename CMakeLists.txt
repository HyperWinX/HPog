cmake_minimum_required(VERSION 3.8)
project(pog
	LANGUAGES CXX
	VERSION 0.5.2
)

## Module path for custom finders.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## Options.
option(POG_BUNDLED_RE2  "Use bundled re2"                   ON)
option(POG_BUNDLED_FMT  "Use bundled fmt"                   ON)
option(POG_TESTS        "Build tests"                       ON)

## Includes.
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

## Require C++20 support.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXPORT_COMPILE_COMMANDS ON)

## Variables.
set(POG_INCLUDE_DIR          "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(POG_INSTALL_CMAKE_DIR    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/cmake/pog")
set(POG_INSTALL_LIB_DIR      "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(POG_INSTALL_INCLUDE_DIR  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
set(POG_CMAKE_CONFIG_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/share/pog-config.cmake.in")
set(POG_CMAKE_CONFIG_FILE    "${CMAKE_CURRENT_BINARY_DIR}/share/pog-config.cmake")
set(POG_CMAKE_VERSION_FILE   "${CMAKE_CURRENT_BINARY_DIR}/share/pog-config-version.cmake")
set(POG_PC_CONFIG_FILE_IN    "${CMAKE_CURRENT_SOURCE_DIR}/share/pog.pc.in")
set(POG_PC_CONFIG_FILE       "${CMAKE_CURRENT_BINARY_DIR}/share/pog.pc")
set(POG_CMAKE_INSTALL_FILES  "${POG_CMAKE_CONFIG_FILE}" "${POG_CMAKE_VERSION_FILE}")
set(POG_PC_INSTALL_FILES     "${POG_PC_CONFIG_FILE}")

## Requirements
### Threads - because of RE2
find_package(Threads REQUIRED)
find_package(re2 REQUIRED)
find_package(fmt REQUIRED)

## Add deps folder to resolve bundled dependencies.
add_subdirectory(tests)

## Library.
add_library(pog INTERFACE)
target_sources(pog INTERFACE ${POG_FILES})
if(MSVC)
	target_compile_options(pog INTERFACE /W3)
else()
	target_compile_options(pog INTERFACE -Wall -Wextra -Wno-empty-body)
endif()
target_include_directories(pog INTERFACE
	"$<BUILD_INTERFACE:${POG_INCLUDE_DIR}>"
	"$<INSTALL_INTERFACE:${POG_INSTALL_INCLUDE_DIR}>"
)
target_link_libraries(pog INTERFACE re2::re2 fmt::fmt)
if(POG_COVERAGE)
	target_compile_options(pog INTERFACE --coverage)
	target_link_libraries(pog INTERFACE --coverage)
endif()

set(RE2_INCLUDE_DIR "${POG_INSTALL_INCLUDE_DIR}/pog")
set(RE2_LIBRARY "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}pog_re2${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(POG_PC_REQUIREMENT "Libs: -L\$\{libdir\} -lpog_re2 -lpthread")

set(FMT_INCLUDE_DIR "${POG_INSTALL_INCLUDE_DIR}/pog")
set(FMT_LIBRARY "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}//${CMAKE_STATIC_LIBRARY_PREFIX}pog_fmt${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(POG_PC_REQUIREMENT "${POG_PC_REQUIREMENT} -lfmt")
