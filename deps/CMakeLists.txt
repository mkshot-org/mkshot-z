# include(FetchContent)

## Dependencies ##
find_package(PkgConfig QUIET REQUIRED)

set(MKSHOT_PC
    vorbisfile
    theoradec
    pixman-1
    physfs
    uchardet
    sdl3
    sdl3-image
    sdl3-ttf
    sdl3-sound
    openal
    ruby-3.4
)

FOREACH(__pc ${MKSHOT_PC})
    pkg_check_modules(MKSHOT_${__pc} REQUIRED IMPORTED_TARGET GLOBAL ${__pc})
    list(APPEND MKSHOT_LIBS PkgConfig::MKSHOT_${__pc})
ENDFOREACH()

set(MKSHOT_LINKLIBS ${MKSHOT_LIBS} PARENT_SCOPE)

## Steamworks SDK ##
IF(MKSHOT_STEAMWORKS_PATH)
    message(STATUS "Searching for Steamworks SDK in '${MKSHOT_STEAMWORKS_PATH}'...")

    set(STEAMWORKS_FOUND OFF)

    IF(STEAMWORKS_FOUND)
        set(MKSHOT_STEAM ON)
        message(STATUS "Steamworks SDK found and validated, enabling Steam features...")
    ELSE()
        message(FATAL_ERROR "Steamworks SDK not found in the given MKSHOT_STEAMWORKS_PATH.")
    ENDIF()
ENDIF()
