##===----------------------------------------------------------------------===##
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
##===----------------------------------------------------------------------===##
#
# Build offloading library libomptarget.so.
#
##===----------------------------------------------------------------------===##

libomptarget_say("Building offloading runtime library libomptarget.")

set(LIBOMPTARGET_SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interface.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rtl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/omptarget.cpp
)

set(LIBOMPTARGET_SRC_FILES ${LIBOMPTARGET_SRC_FILES} PARENT_SCOPE)

include_directories(${LIBOMPTARGET_LLVM_INCLUDE_DIRS})

# Build libomptarget library with libdl dependency. Add LLVMSupport
# dependency if building in-tree with profiling enabled.
if(OPENMP_STANDALONE_BUILD OR (NOT OPENMP_ENABLE_LIBOMPTARGET_PROFILING))
  add_library(bolt-omptarget SHARED ${LIBOMPTARGET_SRC_FILES})
  target_link_libraries(bolt-omptarget
    ${CMAKE_DL_LIBS}
    "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports")
else()
  set(LLVM_LINK_COMPONENTS
    Support
    )
  add_llvm_library(bolt-omptarget SHARED ${LIBOMPTARGET_SRC_FILES}
      LINK_LIBS ${CMAKE_DL_LIBS}
      "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports"
      )
  target_compile_definitions(bolt-omptarget PUBLIC OMPTARGET_PROFILE_ENABLED)
endif()

# libomptarget needs to be set separately because add_llvm_library doesn't
# conform with location configuration of its parent scope.
set_target_properties(bolt-omptarget
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${LIBOMPTARGET_LIBRARY_DIR})

# Install libomptarget under the lib destination folder.
install(TARGETS bolt-omptarget LIBRARY COMPONENT omptarget
  DESTINATION "${OPENMP_INSTALL_LIBDIR}")

# Install aliases
get_target_property(BOLT_LIBOMPTARGET_LIBRARY_DIR bolt-omptarget LIBRARY_OUTPUT_DIRECTORY)
if(BOLT_LIBOMPTARGET_LIBRARY_DIR)
  add_custom_command(TARGET bolt-omptarget POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SHARED_LIBRARY_PREFIX}bolt-omptarget${CMAKE_SHARED_LIBRARY_SUFFIX}
      ${CMAKE_SHARED_LIBRARY_PREFIX}omptarget${CMAKE_SHARED_LIBRARY_SUFFIX}
    WORKING_DIRECTORY ${BOLT_LIBOMPTARGET_LIBRARY_DIR}
  )
endif()
install(CODE "execute_process(COMMAND \"\${CMAKE_COMMAND}\" -E create_symlink \"${CMAKE_SHARED_LIBRARY_PREFIX}bolt-omptarget${CMAKE_SHARED_LIBRARY_SUFFIX}\"
  \"${CMAKE_SHARED_LIBRARY_PREFIX}omptarget${CMAKE_SHARED_LIBRARY_SUFFIX}\" WORKING_DIRECTORY
  \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${OPENMP_INSTALL_LIBDIR})")
