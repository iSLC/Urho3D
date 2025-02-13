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

# Set CMake minimum version. Also set in android/build.gradle for android projects.
cmake_minimum_required (VERSION 3.21)

# Set C++ standard
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_CXX_STANDARD LESS 20)
    message( FATAL_ERROR "At least C++20 is required." )
endif ()

# Set project name and languages
project (Urho3D C CXX)
if (APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)

# Include file that sets up ccache.
include(CCache)

# Include file that sets up all configurable properties
include(UrhoOptions)

# Include common utilitles
include(UrhoCommon)

# Enable testing
if (URHO3D_TESTING)
    include (CTest)
endif ()

# Enable common build options
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CLANG ON)
    set (GNU ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (GCC ON)
    set (GNU ON)
endif()

# Set compiler variable
set ("${CMAKE_CXX_COMPILER_ID}" ON)
set (CMAKE_INSTALL_RPATH "$ORIGIN")
if (GNU OR MINGW)
    if (NOT "$ENV{CI}" STREQUAL "")
        # Disable debugging info completely for CI builds.
        ucm_replace_flag(-g -g0 CONFIG Debug RelWithDebInfo)
    else ()
        # Reduce size of debugging information.
        ucm_replace_flag(-g -g1 CONFIG RelWithDebInfo)
        # Use only debug-friendly optimizations.
        ucm_replace_flag(-O2 -Og CONFIG RelWithDebInfo)
    endif ()
endif ()

# Configure variables
set (URHO3D_URL "https://github.com/iSLC/Urho3D")
set (URHO3D_DESCRIPTION "Urho3D is a free lightweight, cross-platform 2D and 3D game engine implemented in C++ and released under the MIT license. Greatly inspired by OGRE (http://www.ogre3d.org) and Horde3D (http://www.horde3d.org).")
execute_process (COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/GetUrhoRevision.cmake WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE URHO3D_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
string (REGEX MATCH "([^.]+)\\.([^.]+)\\.(.+)" MATCHED "${URHO3D_VERSION}")

# Setup SDK install destinations
if (WIN32)
    set (SCRIPT_EXT .bat)
else ()
    set (SCRIPT_EXT .sh)
endif ()
if (ANDROID)
    # For Android platform, install to a path based on the chosen Android ABI, e.g. libs/armeabi-v7a
    set (LIB_SUFFIX s/${ANDROID_NDK_ABI_NAME})
endif ()

set (DEST_BASE_INCLUDE_DIR include)
set (DEST_INCLUDE_DIR ${DEST_BASE_INCLUDE_DIR}/Urho3D)
set (DEST_BIN_DIR bin)
set (DEST_SHARE_DIR share)
set (DEST_RESOURCE_DIR ${DEST_BIN_DIR})
set (DEST_BUNDLE_DIR ${DEST_SHARE_DIR}/Applications)
set (DEST_ARCHIVE_DIR lib)
set (DEST_PKGCONFIG_DIR ${DEST_ARCHIVE_DIR}/pkgconfig)
set (DEST_THIRDPARTY_HEADERS_DIR ${DEST_INCLUDE_DIR}/ThirdParty)

if (ANDROID)
    set (DEST_LIBRARY_DIR ${DEST_ARCHIVE_DIR})
else ()
    set (DEST_LIBRARY_DIR bin)
endif ()

if (MSVC OR "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set (MULTI_CONFIG_PROJECT ON)
endif ()

if (MULTI_CONFIG_PROJECT)
    set (DEST_BIN_DIR_CONFIG ${DEST_BIN_DIR}/$<CONFIG>)
    set (DEST_LIBRARY_DIR_CONFIG ${DEST_LIBRARY_DIR}/$<CONFIG>)
    set (DEST_ARCHIVE_DIR_CONFIG ${DEST_ARCHIVE_DIR}/$<CONFIG>)
else ()
    set (DEST_BIN_DIR_CONFIG ${DEST_BIN_DIR})
    set (DEST_LIBRARY_DIR_CONFIG ${DEST_LIBRARY_DIR})
    set (DEST_ARCHIVE_DIR_CONFIG ${DEST_ARCHIVE_DIR})
endif ()

if (WIN32)
    if (NOT DEFINED URHO3D_WINVER)
        set (URHO3D_WINVER 0x0601)
    endif ()
    add_definitions(-DWINVER=${URHO3D_WINVER} -D_WIN32_WINNT=${URHO3D_WINVER} -D_WIN32_WINDOWS=${URHO3D_WINVER})
endif ()

if (NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET AND MACOS)
    set (CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
endif ()

# A workaround for Ninja generator putting executables to wrong output dir. This breaks
# VS/Xcode, they work properly without any workarounds.
if ("${CMAKE_GENERATOR}" STREQUAL "Ninja")
    set (_CONFIG "_CONFIG")
endif ()

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_BIN_DIR${_CONFIG}})
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_LIBRARY_DIR${_CONFIG}})
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_ARCHIVE_DIR${_CONFIG}})
endif ()

if (NOT DEFINED VS_DEBUGGER_WORKING_DIRECTORY)
    set (VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()

string(TOLOWER "${CMAKE_GENERATOR_PLATFORM}" CMAKE_GENERATOR_PLATFORM)
set(URHO3D_PLATFORM "${CMAKE_GENERATOR_PLATFORM}")
if (CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(URHO3D_64BIT ON)
    if (NOT URHO3D_PLATFORM)
        set (URHO3D_PLATFORM x64)
    endif ()
else ()
    set(URHO3D_64BIT OFF)
    if (NOT URHO3D_PLATFORM OR "${URHO3D_PLATFORM}" STREQUAL "win32")
        set (URHO3D_PLATFORM x86)
    endif ()
endif ()

if (MULTI_CONFIG_PROJECT)
    set (URHO3D_CONFIG $<CONFIG>)
else ()
    set (URHO3D_CONFIG ${CMAKE_BUILD_TYPE})
endif ()

if (MINGW)
    find_file(DLL_FILE_PATH_1 "libstdc++-6.dll")
    find_file(DLL_FILE_PATH_2 "libgcc_s_seh-1.dll")
    find_file(DLL_FILE_PATH_3 "libwinpthread-1.dll")
    foreach (DLL_FILE_PATH ${DLL_FILE_PATH_1} ${DLL_FILE_PATH_2} ${DLL_FILE_PATH_3})
        if (DLL_FILE_PATH)
            # Copies dlls to bin or tools.
            file (COPY ${DLL_FILE_PATH} DESTINATION ${CMAKE_BINARY_DIR}/${DEST_BIN_DIR_CONFIG})
            if (NOT URHO3D_STATIC_RUNTIME)
                file (COPY ${DLL_FILE_PATH} DESTINATION ${CMAKE_BINARY_DIR}/${DEST_BIN_DIR_CONFIG})
            endif ()
        endif ()
    endforeach ()
endif ()

if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
    set (HOST_LINUX 1)
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set (HOST_WIN32 1)
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    set (HOST_MACOS 1)
endif ()

if (URHO3D_SSE)
    if (DESKTOP)
        string (TOLOWER "${URHO3D_SSE}" URHO3D_SSE)
        if (MSVC)
            list (APPEND VALID_SSE_OPTIONS sse sse2 avx avx2)
        else ()
            list (APPEND VALID_SSE_OPTIONS sse sse2 sse3 sse4 sse4a sse4.1 sse4.2 avx avx2)
        endif ()
        list (FIND VALID_SSE_OPTIONS "${URHO3D_SSE}" SSE_INDEX)
        if (SSE_INDEX EQUAL -1)
            set (URHO3D_SSE sse2)
        endif ()
        if (MSVC)
            string (TOUPPER "${URHO3D_SSE}" URHO3D_SSE)
        endif ()
    else ()
        set (URHO3D_SSE OFF)
    endif ()
endif ()

# Workarounds. They are mirrored in Urho3D target build script so that projects linking to it would inherit settings.
if (ANDROID)
    set (URHO3D_CXX_STANDARD ${CMAKE_CXX_STANDARD})
    ucm_add_flags(CXX -std=gnu++${CMAKE_CXX_STANDARD})
    # NDK bundles old cmake and insists on using it, That version chokes on standard being set to 17 while
    # crosscompiler works just fine.
    unset (CMAKE_CXX_STANDARD)
endif ()
if (CLANG)
    if (APPLE)
        # Ensure that everything built by this project use these libraries. Urho3D target also exports them so all downstream
        # projects linking to the engine get them as well.
        ucm_add_flags(C CXX -stdlib=libc++)
        link_libraries(c++ c++abi)
    elseif (DESKTOP AND UNIX)
        # Android ndk handles linking runtime library. Emscripten does not need these.
        link_libraries(stdc++ m)
    endif ()
endif ()

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (BUILD_SHARED_LIBS)
    set (CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (URHO3D_SSE AND NOT MSVC)
    # Build engine and it's dependencies with SSE/SSE2 enabled.
    ucm_add_flags(-msse -msse2)
endif ()

if (NOT "$ENV{CI}" STREQUAL "")
    if (MSVC)
        ucm_replace_flag("/W[0-3]" "/W0" REGEX)
    else ()
        ucm_add_flags("-w")
    endif ()
elseif (MSVC)
    ucm_replace_flag("/W[0-3]" "/W1" REGEX)
endif ()

if (DESKTOP)
    # Mobile toolchains manage runtime libraries themselves, we should not interfere.
    if (URHO3D_STATIC_RUNTIME)
        ucm_set_runtime(STATIC)
    else ()
        ucm_set_runtime(DYNAMIC)
    endif ()
endif ()

add_subdirectory(Source)

# Print current build configuration
message(STATUS "Urho3D Configuration:")
if (URHO3D_STATIC_RUNTIME)
    message(STATUS "  Runtime         STATIC")
else ()
    message(STATUS "  Runtime         SHARED")
endif ()
if (BUILD_SHARED_LIBS)
    message(STATUS "  Library         SHARED")
else ()
    message(STATUS "  Library         STATIC")
endif ()
message(STATUS "  Graphics        ${URHO3D_GRAPHICS_API}")
message(STATUS "  SSE             ${URHO3D_SSE}")
message(STATUS "  2D              ${URHO3D_URHO2D}")
message(STATUS "  Physics2D       ${URHO3D_PHYSICS2D}")
message(STATUS "  IK              ${URHO3D_IK}")
message(STATUS "  Threading       ${URHO3D_THREADING}")
message(STATUS "  Navigation      ${URHO3D_NAVIGATION}")
message(STATUS "  Network         ${URHO3D_NETWORK}")
message(STATUS "  Physics         ${URHO3D_PHYSICS}")
message(STATUS "  Samples         ${URHO3D_SAMPLES}")
message(STATUS "  Testing         ${URHO3D_TESTING}")
if (WIN32)
    message(STATUS "  MiniDumps       ${URHO3D_MINIDUMPS}")
endif()
message(STATUS "  Precompiled     ${URHO3D_PCH}")
message(STATUS "Developer options:")
message(STATUS "  Logging         ${URHO3D_LOGGING}")
message(STATUS "  Profiling       ${URHO3D_PROFILING}")
message(STATUS "  Extras          ${URHO3D_EXTRAS}")
message(STATUS "  Tools           ${URHO3D_TOOLS}")
if (TARGET Profiler)
    message(STATUS "     Profiler GUI ${URHO3D_PROFILING}")
endif ()

# clang-tidy target
find_program(CLANG_TIDY clang-tidy)
if (CLANG_TIDY)
    file (GLOB_RECURSE SOURCE_FILES
        Source/Samples/**.cpp Source/Samples/**.h Source/Samples/**.hpp Source/Samples/**.inl
        Source/Urho3D/**.cpp Source/Urho3D/**.h Source/Urho3D/**.hpp
        Source/Tools/**.cpp Source/Tools/**.h Source/Tools/**.hpp
    )
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} -p . -export-fixes=clang-tidy.yml -fix ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_target_properties(tidy PROPERTIES EXCLUDE_FROM_ALL ON EXCLUDE_FROM_DEFAULT_BUILD ON)
endif()

################################################### SDK preparation ####################################################
if (MINI_URHO)
    return ()
endif ()

# Install CMake modules and toolchains provided by and for Urho3D
install (DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/Toolchains/ DESTINATION ${DEST_SHARE_DIR}/CMake/Toolchains)    # Note: the trailing slash is significant
install (DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/Modules/ DESTINATION ${DEST_SHARE_DIR}/CMake/Modules/)
# Install CMake build scripts
file (GLOB CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/Script/*${SCRIPT_EXT})
install (PROGRAMS ${CMAKE_SCRIPTS} DESTINATION ${DEST_SHARE_DIR}/Scripts)
# Install data files
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if (NOT ANDROID)
    # Multi-config projects store data files in parent dir of binary files.
    string (REPLACE "/$<CONFIG>" "" RUNTIME_OUTPUT_DIRECTORY_NO_CONFIG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    foreach(dir Data CoreData Autoload)
        create_symlink(${CMAKE_CURRENT_SOURCE_DIR}/bin/${dir} ${RUNTIME_OUTPUT_DIRECTORY_NO_CONFIG}/${dir})
        # /${dir}/../ trick is used for installing Autoload subdirs.
        install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${dir} DESTINATION ${CMAKE_INSTALL_PREFIX}/${DEST_BIN_DIR}/${dir}/../)
    endforeach()
endif ()
