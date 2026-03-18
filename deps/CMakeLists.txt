# include(FetchContent)

## Dependencies ##
find_package(PkgConfig REQUIRED)
pkg_check_modules(MKSHOT_VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)
pkg_check_modules(MKSHOT_THEORADEC REQUIRED IMPORTED_TARGET theoradec)
pkg_check_modules(MKSHOT_PIXMAN REQUIRED IMPORTED_TARGET pixman-1)

find_package(PhysFS REQUIRED)
find_package(uchardet REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_sound REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Ruby REQUIRED)

set(MKSHOT_LINKLIBS
    PkgConfig::MKSHOT_VORBISFILE
    PkgConfig::MKSHOT_THEORADEC
    PkgConfig::MKSHOT_PIXMAN
    physfs
    uchardet
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_sound::SDL3_sound
    OpenAL::OpenAL
    ${RUBY_LIBRARIES}
)

## Steamworks SDK ##
IF(MKSHOT_STEAMWORKS_PATH)
    message(STATUS "Searching for Steamworks SDK in '${MKSHOT_STEAMWORKS_PATH}'...")

    # look for the actual header to verify the path
    find_path(MKSHOT_STEAMWORKS_PATH steam/steam_api.h
        PATHS "${MKSHOT_STEAMWORKS_PATH}/public"
        NO_DEFAULT_PATH
    )

    IF(STEAM_INCLUDE_DIR)
        set(MKSHOT_STEAM ON)
        message(STATUS "The Steamworks SDK has been found and validated, enabling Steam features.")
    ELSE()
        message(FATAL_ERROR "MKSHOT_STEAMWORKS_PATH was set, but steam/steam_api.h was not found!")
    ENDIF()
ENDIF()
