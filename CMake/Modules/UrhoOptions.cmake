#
# Copyright (c) 2008-2017 the Urho3D project.
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

if (NOT MINI_URHO)
    # Source environment
    if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        execute_process(COMMAND cmd /c set OUTPUT_VARIABLE ENVIRONMENT)
    else ()
        execute_process(COMMAND env OUTPUT_VARIABLE ENVIRONMENT)
    endif ()
    string(REGEX REPLACE "=[^\n]*\n?" ";" ENVIRONMENT "${ENVIRONMENT}")
    set(IMPORT_URHO3D_VARIABLES_FROM_ENV BUILD_SHARED_LIBS MINI_URHO)
    foreach(key ${ENVIRONMENT})
        list (FIND IMPORT_URHO3D_VARIABLES_FROM_ENV ${key} _index)
        if ("${key}" MATCHES "^(URHO3D_|CMAKE_|ANDROID_).+" OR ${_index} GREATER -1)
            if (NOT DEFINED ${key})
                set (${key} $ENV{${key}} CACHE STRING "" FORCE)
            endif ()
        endif ()
    endforeach()
endif ()

# https://cmake.org/cmake/help/v3.18/policy/CMP0077.html
# Note that cmake_minimum_required() + project() resets policies, so dependencies using lower CMake version would not
# properly accept options before we add_subdirectory() them without setting this policy to NEW __in their build script__.
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
endif ()

foreach (feature ${URHO3D_FEATURES})
    set (URHO3D_${feature} ON)
endforeach()

include(CMakeDependentOption)

# Set platform variables
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set (LINUX ON CACHE BOOL "" FORCE)
endif ()

if (ANDROID OR IOS)
    set (MOBILE ON CACHE BOOL "" FORCE)
elseif (APPLE OR "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    set (APPLE ON CACHE BOOL "" FORCE)
endif ()

if (APPLE AND NOT IOS)
    set (MACOS ON CACHE BOOL "" FORCE)
endif ()

if ((WIN32 OR LINUX OR MACOS) AND NOT MOBILE)
    set (DESKTOP ON CACHE BOOL "" FORCE)
endif ()

# Build properties
option(BUILD_SHARED_LIBS                        "Build engine as shared library."       OFF)
option(URHO3D_ENABLE_ALL                        "Enable (almost) all engine features."  ON)
option(URHO3D_STATIC_RUNTIME                    "Link to static runtime"               OFF)

# No need to check if cmake supports PCH since we have a hard cmake version requirement for the whole poject that's well above that version
option(URHO3D_PCH                           "Enable precompiled header"                              ON)

# Subsystems
option                (URHO3D_IK                 "Inverse kinematics subsystem enabled"                  ${URHO3D_ENABLE_ALL})
option                (URHO3D_LOGGING            "Enable logging subsystem"                              ${URHO3D_ENABLE_ALL})
option                (URHO3D_NAVIGATION         "Navigation subsystem enabled"                          ${URHO3D_ENABLE_ALL})
option                (URHO3D_NETWORK            "Networking subsystem enabled"                          ${URHO3D_ENABLE_ALL})
option                (URHO3D_PHYSICS            "Physics subsystem enabled"                             ${URHO3D_ENABLE_ALL})
option                (URHO3D_URHO2D             "2D subsystem enabled"                                  ${URHO3D_ENABLE_ALL})
option                (URHO3D_PHYSICS2D          "2D physics subsystem enabled"                          ${URHO3D_ENABLE_ALL})

# Features
option                (URHO3D_DEBUG_ASSERT       "Enable Urho3D assert macros"                           ${URHO3D_ENABLE_ALL}                                    )
cmake_dependent_option(URHO3D_FILEWATCHER        "Watch filesystem for resource changes"                 ${URHO3D_ENABLE_ALL} "URHO3D_THREADING"              OFF)
option                (URHO3D_HASH_DEBUG         "Enable StringHash name debugging"                      ${URHO3D_ENABLE_ALL}                                    )
option                (URHO3D_MONOLITHIC_HEADER  "Create Urho3DAll.h which includes all engine headers." OFF                                                     )
cmake_dependent_option(URHO3D_MINIDUMPS          "Enable writing minidumps on crash"                     ${URHO3D_ENABLE_ALL} "MSVC"                          OFF)
option                (URHO3D_THREADING          "Enable multithreading"                                 ${URHO3D_ENABLE_ALL})
cmake_dependent_option(URHO3D_TESTING            "Enable unit tests"                                     OFF                  "NOT MOBILE"                    OFF)

# Misc
option(URHO3D_PLAYER                            "Build player application"                              ${URHO3D_ENABLE_ALL})
cmake_dependent_option(URHO3D_EDITOR            "Build editor application"                              ${URHO3D_ENABLE_ALL} "DESKTOP"                       OFF)
cmake_dependent_option(URHO3D_EXTRAS            "Build extra tools"                                     ${URHO3D_ENABLE_ALL} "NOT MOBILE"                    OFF)
cmake_dependent_option(URHO3D_TOOLS             "Tools enabled"                                         ${URHO3D_ENABLE_ALL} "DESKTOP"                       OFF)
option(URHO3D_SAMPLES                           "Build samples"                                         OFF)
cmake_dependent_option(URHO3D_MERGE_STATIC_LIBS "Merge third party dependency libs to Urho3D.a"         OFF "NOT BUILD_SHARED_LIBS"                          OFF)
option(URHO3D_NO_EDITOR_PLAYER_EXE              "Do not build editor or player executables."            OFF)
option(URHO3D_SSL                               "Enable OpenSSL support"                                OFF)

if (WIN32)
    set(URHO3D_GRAPHICS_API D3D11 CACHE STRING "Graphics API")
    set_property(CACHE URHO3D_GRAPHICS_API PROPERTY STRINGS D3D9 D3D11 OpenGL)
    option(URHO3D_WIN32_CONSOLE "Show log messages in win32 console"                     OFF)
elseif (IOS OR ANDROID)
    set(URHO3D_GRAPHICS_API GLES2 CACHE STRING "Graphics API")
    set_property(CACHE URHO3D_GRAPHICS_API PROPERTY STRINGS GLES2 GLES3)
else ()
    set(URHO3D_GRAPHICS_API OpenGL)
endif ()
string(TOUPPER "${URHO3D_GRAPHICS_API}" URHO3D_GRAPHICS_API)
set (URHO3D_${URHO3D_GRAPHICS_API} ON)
if (URHO3D_GLES2 OR URHO3D_GLES3)
    set (URHO3D_OPENGL ON)
endif ()

# Implicit configuration
if (ANDROID OR IOS)
    set (URHO3D_SSE OFF)
elseif ((URHO3D_TOOLS OR URHO3D_EDITOR) AND NOT MINI_URHO)
    set (URHO3D_FILEWATCHER ON)
    set (URHO3D_LOGGING ON)
    set (URHO3D_HASH_DEBUG ON)
endif ()

if (ANDROID)
    set (SDL_CPUINFO ON)
endif ()
