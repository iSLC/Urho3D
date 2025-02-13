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
  src/asm-bits.c
  src/cpuid_main.c
  src/libcpuid_util.c
  src/msrdriver.c
  src/rdcpuid.c
  src/rdmsr.c
  src/rdtsc.c
  src/recog_amd.c
  src/recog_arm.c
  src/recog_centaur.c
  src/recog_intel.c
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

# Windows specific source files
if(WIN32)
  list(APPEND SOURCE_FILES src/msrdriver.c)
endif()

# Define generated source files
if(MSVC AND URHO3D_64BIT)
  enable_language(ASM_MASM)
  list(APPEND SOURCE_FILES src/masm-x64.asm)
endif()

# Create the library
add_library(LibCPUID STATIC ${SOURCE_FILES})
# Compile time definitions
target_compile_definitions(LibCPUID PRIVATE VERSION="0.7.1")
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
