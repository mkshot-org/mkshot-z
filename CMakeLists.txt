## Cool info ##
cmake_minimum_required(VERSION 3.25)

project(mkshot-z
    DESCRIPTION "OneShot (2016) engine reimplementation for modders"
    LANGUAGES "C" "CXX"
)

IF(APPLE)
   enable_language("OBJCXX")
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

## Environment checks ##
IF(WIN32)
    # and no, msvc isn't supported
    # (never will be)
    IF(Not DEFINED ENV{MSYSTEM})
        message(FATAL_ERROR
        "Building on Windows requires you to use MSYS2: <https://www.msys2.org>.\n"
        "Please consult the documentation before proceeding.")
    ENDIF()
ELSEIF(APPLE)
    message(FATAL_ERROR "macOS isn't supported at the moment.")
ELSEIF(NOT LINUX)
    message(FATAL_ERROR "This operating system isn't supported. 'Detected: ${CMAKE_SYSTEM_NAME}'.")
ENDIF()

set(MKSHOT_GCC OFF)
set(MKSHOT_CLANG OFF)

message(STATUS "Detected: ${CMAKE_CXX_COMPILER_ID}")
IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MKSHOT_GCC ON)
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "^Clang|AppleClang")
    set(MKSHOT_CLANG ON)
ELSE()
    message(FATAL_ERROR "mkshot-z only supports GCC or Clang.")
ENDIF()


## Options ##
option(MKSHOT_DYNAMIC "Build and link mkshot-z dependencies dynamically." OFF)
option(MKSHOT_FORCE_32BIT "Force the compiler to build in 32-bit mode." OFF)
option(MKSHOT_GLES2 "Use OpenGL ES 2.0 instead of OpenGL." OFF)
option(MKSHOT_HTTP "Add support for HTTP requests." OFF)
option(MKSHOT_KEEP_CWD "Keep Current Working Directory on startup." OFF)

# there are no special conditions for dynamic linking
# so we'll use MKSHOT_STATIC for conditions instead
IF(MKSHOT_DYNAMIC)
    set(MKSHOT_STATIC OFF)
ELSE()
    set(MKSHOT_STATIC ON)
ENDIF()

# Steam
set(MKSHOT_STEAMWORKS_PATH "" CACHE PATH "Path to Steamworks SDK directory. Provide this to enable the Steam features.")
set(MKSHOT_STEAM_APPID "" CACHE STRING "Steam AppID value. Set this to use 'SteamAPI_RestartAppIfNecessary()'.")
option(MKSHOT_STEAMSHIM_DEBUG "Whether to show Steamshim debug output console." OFF)
set(MKSHOT_STEAM OFF)

## Prerequisites ##
# get short git hash (we use that instead of versioning)
find_package(Git QUIET)
set(MKSHOT_VER_HASH "no git hash found :p")

IF(GIT_EXECUTABLE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE _mkshot_verhash_output
        RESULT_VARIABLE _mkshot_verhash_exitcode
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    IF(_mkshot_verhash_exitcode EQUAL 0)
        set(MKSHOT_VER_HASH "${_mkshot_verhash_output}")
        message(STATUS "Git hash: ${_mkshot_verhash_output}")
    ELSE()
        message(WARNING "Failed getting the Git hash. Exit code: ${_mkshot_verhash_exitcode}")
        message(WARNING "Hash value will fallback to '${MKSHOT_VER_HASH}'.")
    ENDIF()
ELSE()
    message(WARNING "Git executable not found. Hash value will fallback to '${MKSHOT_VER_HASH}'.")
ENDIF()

# subdirs
add_subdirectory(deps)
add_subdirectory(embed)
add_subdirectory(src)
