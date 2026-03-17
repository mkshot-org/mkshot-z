set(MKSHOT_EMBED
    "assets/gamecontrollerdb.txt"
    "assets/wqymicrohei.ttf"

    "scripts/rgss1.rb"
    "scripts/rgss2.rb"
    "scripts/rgss3.rb"

    "shaders/common.hpp"
    "shaders/bicubic.frag"
    "shaders/bitmap-blit.frag"
    "shaders/blur.frag"
    "shaders/flash-map.frag"
    "shaders/flat-color.frag"
    "shaders/gray.frag"
    "shaders/hue.frag"
    "shaders/lanczos3.frag"
    "shaders/obscured.frag"
    "shaders/plane.frag"
    "shaders/simple-alpha-uni.frag"
    "shaders/simple-alpha.frag"
    "shaders/simple-color.frag"
    "shaders/simple.frag"
    "shaders/sprite.frag"
    "shaders/tilemap.frag"
    "shaders/trans-simple.frag"
    "shaders/trans.frag"
    "shaders/xbrz.frag"
    "shaders/blur-h.vert"
    "shaders/blur-v.vert"
    "shaders/minimal.vert"
    "shaders/simple-color.vert"
    "shaders/simple-matrix.vert"
    "shaders/simple.vert"
    "shaders/sprite.vert"
    "shaders/tilemap-vx.vert"
    "shaders/tilemap.vert"
)

# this stuff in particular is copied into the install directory
set(MKSHOT_ASSETS "${CMAKE_SOURCE_DIR}/LICENSE" PARENT_SCOPE)

IF(WIN32)
    list(APPEND MKSHOT_EMBED assets/icon.ico)
ELSEIF(LINUX)
    list(APPEND MKSHOT_EMBED assets/icon.png)
#ELSEIF(APPLE)
    # not yet :eyes:
ENDIF()

IF(MKSHOT_STEAM AND MKSHOT_STEAM_APPID)
    file(WRITE assets/steam_appid.txt "${MKSHOT_STEAM_APPID}")
    list(APPEND MKSHOT_ASSETS assets/steam_appid.txt)
ENDIF()

file(MAKE_DIRECTORY include)
FOREACH(__input_fullpath ${MKSHOT_EMBED})
    string(REGEX REPLACE "(assets|scripts|shaders)/" "" __input_file "${__input_fullpath}")
    set(__output_file include/${__input_file}.h)
    
    message(STATUS "Generating a C-style array from ${__input_fullpath} for embedding into the executable...")
    execute_process(
        COMMAND xxd -i ${__input_fullpath} ${__output_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
ENDFOREACH()
