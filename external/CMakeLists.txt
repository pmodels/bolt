# -*- Mode: CMakeLists.txt; -*-
#
# See LICENSE.txt in top-level directory.
#

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Check if the built-in Argobots needs to be used
set(LIBOMP_USE_ARGOBOTS FALSE CACHE BOOL
  "Use Argobots (http://www.argobots.org) as threading model?")
if(${LIBOMP_USE_ARGOBOTS} AND (NOT LIBOMP_ARGOBOTS_INSTALL_DIR))
  set(LIBOMP_USE_BUILTIN_ARGOBOTS TRUE CACHE BOOL "")
else()
  set(LIBOMP_USE_BUILTIN_ARGOBOTS FALSE CACHE BOOL "")
endif()

if(${LIBOMP_USE_BUILTIN_ARGOBOTS})
  # Use the built-in Argobots
  set(ABT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/argobots)
  set(ABT_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/argobots)
  set(ABT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/argobots/install)
  set(ABT_AUTOGEN ${ABT_DIR}/autogen.sh)
  set(ABT_CONFIGURE ${ABT_DIR}/configure)
  if ((NOT EXISTS ${ABT_CONFIGURE}) OR (${ABT_AUTOGEN} IS_NEWER_THAN ${ABT_CONFIGURE}))
    execute_process(
      COMMAND ./autogen.sh
      WORKING_DIRECTORY ${ABT_DIR}
    )
  endif()

  include(ExternalProject)
  ExternalProject_Add(libabt
    SOURCE_DIR ${ABT_DIR}
    BINARY_DIR ${ABT_BUILD_DIR}
    CONFIGURE_COMMAND ${ABT_CONFIGURE} --prefix=${ABT_INSTALL_DIR} CC=${CMAKE_C_COMPILER}
  )
  # FIXME: pkfconfig is not properly set since the built-in Argobots is
  # once installed to a different path.  Temporarily pkgconfig is not installed.
  install(DIRECTORY ${ABT_INSTALL_DIR}/lib
          DESTINATION ${CMAKE_INSTALL_PREFIX}
          PATTERN pkgconfig EXCLUDE)
  install(DIRECTORY ${ABT_INSTALL_DIR}/include
          DESTINATION ${CMAKE_INSTALL_PREFIX})

  set(LIBOMP_ARGOBOTS_INSTALL_DIR ${ABT_INSTALL_DIR} PARENT_SCOPE)
else()
  set(LIBOMP_ARGOBOTS_INSTALL_DIR /usr/local CACHE PATH
    "Install path for Argobots")
endif()
