#
# Copyright (c) 2008-2022 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Define source files
set(SOURCE_FILES
  src/cpuid_main.c
  src/recog_amd.c
  src/recog_arm.c
  src/recog_centaur.c
  src/recog_intel.c
  src/rdtsc.c
  src/libcpuid_util.c
  src/rdmsr.c
  src/asm-bits.c
  src/rdcpuid.c
  include/LibCpuId/amd_code_t.h
  include/LibCpuId/asm-bits.h
  include/LibCpuId/centaur_code_t.h
  include/LibCpuId/intel_code_t.h
  include/LibCpuId/libcpuid.h
  include/LibCpuId/libcpuid_arm_driver.h
  include/LibCpuId/libcpuid_constants.h
  include/LibCpuId/libcpuid_ctype.h
  include/LibCpuId/libcpuid_internal.h
  include/LibCpuId/libcpuid_types.h
  include/LibCpuId/libcpuid_util.h
  include/LibCpuId/rdcpuid.h
  include/LibCpuId/rdtsc.h
  include/LibCpuId/recog_amd.h
  include/LibCpuId/recog_arm.h
  include/LibCpuId/recog_centaur.h
  include/LibCpuId/recog_intel.h
)

set(LIBCPUID_CURRENT 17)
set(LIBCPUID_AGE 0)
set(LIBCPUID_REVISION 1)

if(MSVC)
  set(LIBCPUID_SHARED OFF)
else()
  set(LIBCPUID_SHARED ON)
endif()

# Windows specific source files
if(WIN32)
  list(APPEND SOURCE_FILES src/msrdriver.c)
endif()

enable_language(C ASM_MASM)

# Define generated source files
if(MSVC AND URHO3D_64BIT)
  enable_language(ASM_MASM)
  list(APPEND SOURCE_FILES src/masm-x64.asm)
endif()

#target_compile_definitions(LibCPUID PUBLIC LIBCPUID_DISABLE_DEPRECATED=1)

# pthreads library
if(${CMAKE_SYSTEM_NAME} STREQUAL "DragonFly" OR ${CMAKE_SYSTEM_NAME} STREQUAL "NetBSD")
  find_package(Threads REQUIRED)
endif()

# Create the library
add_library(LibCPUID STATIC ${SOURCE_FILES})
set_property(TARGET LibCPUID PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_link_libraries(LibCPUID ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET LibCPUID PROPERTY C_STANDARD 99)
set_property(TARGET LibCPUID PROPERTY CXX_STANDARD 11)

# check if popcount64 is available
include(CheckSymbolExists)
check_symbol_exists(popcount64 "string.h" HAVE_POPCOUNT64)
if(HAVE_POPCOUNT64)
  target_compile_definitions(LibCPUID PRIVATE HAVE_POPCOUNT64=1)
endif(HAVE_POPCOUNT64)

# check if auxiliary vector is available
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  check_symbol_exists(getauxval "sys/auxv.h" HAVE_GETAUXVAL)
  if(HAVE_GETAUXVAL)
    target_compile_definitions(LibCPUID PRIVATE HAVE_GETAUXVAL=1)
  endif(HAVE_GETAUXVAL)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
  check_symbol_exists(elf_aux_info "sys/auxv.h" HAVE_ELF_AUX_INFO)
  if(HAVE_ELF_AUX_INFO)
    target_compile_definitions(LibCPUID PRIVATE HAVE_ELF_AUX_INFO=1)
  endif(HAVE_ELF_AUX_INFO)
endif()

# Compile time definitions
target_compile_definitions(LibCPUID PRIVATE VERSION="0.7.1")
set_target_properties(LibCPUID PROPERTIES VERSION "${LIBCPUID_CURRENT}.${LIBCPUID_AGE}.${LIBCPUID_REVISION}")
if (HAVE_STDINT_H)
    target_compile_definitions(LibCPUID PRIVATE HAVE_STDINT_H=1)
endif ()
# Unix specific compile time definitions
if(UNIX)
  target_compile_definitions(LibCPUID PRIVATE _GNU_SOURCE)
endif()
# Include folders
target_include_directories(LibCPUID
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/LibCPUID>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (NOT MINI_URHO)
    if (NOT URHO3D_MERGE_STATIC_LIBS)
        install(TARGETS LibCPUID EXPORT Urho3D ARCHIVE DESTINATION ${DEST_ARCHIVE_DIR_CONFIG})
    endif ()
endif ()
