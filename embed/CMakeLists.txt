set(MKSHOT_EMBED PATH
    gamecontrollerdb.txt
    wqymicrohei.ttf

    scripts/rgss1.rb
    scripts/rgss2.rb
    scripts/rgss3.rb

    shaders/bicubic.frag
    shaders/bitmap-blit.frag
    shaders/blur.frag
    shaders/flash-map.frag
    shaders/flat-color.frag
    shaders/gray.frag
    shaders/hue.frag
    shaders/lanczos3.frag
    shaders/obscured.frag
    shaders/plane.frag
    shaders/simple-alpha-uni.frag
    shaders/simple-alpha.frag
    shaders/simple-color.frag
    shaders/simple.frag
    shaders/sprite.frag
    shaders/tilemap.frag
    shaders/trans-simple.frag
    shaders/trans.frag
    shaders/xbrz.frag
    shaders/blur-h.vert
    shaders/blur-v.vert
    shaders/minimal.vert
    shaders/simple-color.vert
    shaders/simple-matrix.vert
    shaders/simple.vert
    shaders/sprite.vert
    shaders/tilemap-vx.vert
    shaders/tilemap.vert
)

# this stuff in particular is copied into the install directory
set(MKSHOT_ASSETS ${CMAKE_SOURCE_DIR}/LICENSE)

IF(WIN32)
    list(APPEND MKSHOT_EMBED icon.ico)
ELSEIF(LINUX)
    list(APPEND MKSHOT_EMBED icon.png)
#ELSEIF(APPLE)
    # not yet :eyes:
ENDIF()

IF(MKSHOT_STEAM And NOT MKSHOT_STEAM_APPID STREQUAL "")
    file(WRITE "steam_appid.txt" "${MKSHOT_STEAM_APPID}")
    list(APPEND MKSHOT_ASSETS steam_appid.txt)
ENDIF()

function(embed_assets MKSHOT_EMBED_INPUT)
    add_custom_command(
        OUTPUT "${MKSHOT_EMBED_INPUT}.h"
        COMMAND ${CMAKE_COMMAND} 
                -D__mkshot_embed_input="${MKSHOT_EMBED_INPUT}" 
                -D__mkshot_embed_output="include/${MKSHOT_EMBED_INPUT}.h" 
                -P "${CMAKE_SOURCE_DIR}/cmake/EmbedAssets.cmake"
        DEPENDS "${INPUT_FILE}" "${CMAKE_SOURCE_DIR}/cmake/EmbedAssets.cmake"
        COMMENT "Generating 'include/${MKSHOT_EMBED_INPUT}.h' from '${MKSHOT_EMBED_INPUT}' for embedding..."
    )
endfunction()
