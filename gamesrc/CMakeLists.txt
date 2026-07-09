cmake_minimum_required(VERSION 3.31)
project(openarena_game_modules LANGUAGES C)

include(CTest)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(OA_BASEGAME "openarena" CACHE STRING "Base game / fs_game name embedded in native game modules")

# Put artifacts next to the mod so tooling can copy them easily
set(OA_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/..")
set(OA_UI_OUTPUT_DIR "${OA_OUTPUT_DIR}/ui")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")

# Allow callers to point at an explicit engine checkout.
set(ENGINE_ROOT "" CACHE PATH "Path to an id Tech 3 engine checkout")
if(NOT ENGINE_ROOT)
    set(_oa_engine_root_candidates
        "${CMAKE_SOURCE_DIR}/../../idtech3"
        "${CMAKE_SOURCE_DIR}/../../../idtech3"
        "${CMAKE_SOURCE_DIR}/../../..")
    foreach(_oa_engine_root_candidate IN LISTS _oa_engine_root_candidates)
        get_filename_component(_oa_engine_root_candidate_abs "${_oa_engine_root_candidate}" ABSOLUTE)
        if(EXISTS "${_oa_engine_root_candidate_abs}/engine/core/q_shared.h"
           OR EXISTS "${_oa_engine_root_candidate_abs}/src/qcommon/q_shared.h")
            set(ENGINE_ROOT "${_oa_engine_root_candidate_abs}")
            break()
        endif()
    endforeach()
    unset(_oa_engine_root_candidate_abs)
    unset(_oa_engine_root_candidates)
endif()

set(ENGINE_INCLUDE_DIRS "")
set(OA_IDTECH3_FORK_COMPAT OFF)

function(add_engine_include_dir dir)
    if(EXISTS "${dir}")
        list(APPEND ENGINE_INCLUDE_DIRS "${dir}")
        set(ENGINE_INCLUDE_DIRS "${ENGINE_INCLUDE_DIRS}" PARENT_SCOPE)
    endif()
endfunction()

# Stock / ioq3-style trees.
add_engine_include_dir("${ENGINE_ROOT}/src/qcommon")
add_engine_include_dir("${ENGINE_ROOT}/src/game")
add_engine_include_dir("${ENGINE_ROOT}/src/cgame")
add_engine_include_dir("${ENGINE_ROOT}/src/ui")
add_engine_include_dir("${ENGINE_ROOT}/src/renderercommon")
add_engine_include_dir("${ENGINE_ROOT}/src/client")

# timfox/idtech3 fork layout.
add_engine_include_dir("${ENGINE_ROOT}/engine/core")
add_engine_include_dir("${ENGINE_ROOT}/runtime/game")
add_engine_include_dir("${ENGINE_ROOT}/runtime/cgame")
add_engine_include_dir("${ENGINE_ROOT}/runtime/ui")
add_engine_include_dir("${ENGINE_ROOT}/runtime/shared")
add_engine_include_dir("${ENGINE_ROOT}/renderers/common")

if(EXISTS "${ENGINE_ROOT}/engine/core/q_shared.h")
    set(OA_IDTECH3_FORK_COMPAT ON)
endif()

if(NOT ENGINE_INCLUDE_DIRS)
    message(WARNING "No engine include directories were found under ENGINE_ROOT='${ENGINE_ROOT}'. Building with local headers only.")
endif()

message(STATUS "ENGINE_ROOT: ${ENGINE_ROOT}")
message(STATUS "Library output: ${OA_OUTPUT_DIR}/vm")
message(STATUS "timfox/idtech3 fork compat: ${OA_IDTECH3_FORK_COMPAT}")

# Local headers MUST come first to avoid using engine's standard headers
include_directories("${CMAKE_SOURCE_DIR}/game")
include_directories("${CMAKE_SOURCE_DIR}/cgame")
include_directories("${CMAKE_SOURCE_DIR}/ui")
include_directories("${CMAKE_SOURCE_DIR}/q3_ui")
include_directories("${CMAKE_SOURCE_DIR}/qcommon")
include_directories(${ENGINE_INCLUDE_DIRS})

add_compile_options(-fPIC -fvisibility=hidden -Wall -Wextra -Wno-unused-parameter)

if(OA_IDTECH3_FORK_COMPAT)
    add_compile_definitions(IDTECH3_FORK_COMPAT=1)
endif()

set(ARCH_SUFFIX "" CACHE STRING "Override the native module suffix used in output names")

if(NOT ARCH_SUFFIX AND UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(ARCH_SUFFIX "x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86")
        set(ARCH_SUFFIX "i386")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(ARCH_SUFFIX "aarch64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(ARCH_SUFFIX "arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        set(ARCH_SUFFIX "arm")
    endif()
elseif(NOT ARCH_SUFFIX AND WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_SUFFIX "x86_64")
    else()
        set(ARCH_SUFFIX "x86")
    endif()
endif()

if(NOT ARCH_SUFFIX)
    message(FATAL_ERROR "Unable to determine architecture suffix")
endif()

# Game (qagame) source files
set(GAME_SOURCES
    game/ai_chat.c
    game/ai_cmd.c
    game/ai_dmnet.c
    game/ai_dmq3.c
    game/ai_main.c
    game/ai_team.c
    game/ai_vcmd.c
    game/bg_alloc.c
    game/bg_lib.c
    game/bg_misc.c
    game/bg_pmove.c
    game/bg_slidemove.c
    game/g_active.c
    game/g_admin.c
    game/g_arenas.c
    game/g_bot.c
    game/g_client.c
    game/g_cmds.c
    game/g_cmds_ext.c
    game/g_combat.c
    game/g_elimination.c
    game/g_fileops.c
    game/g_items.c
    game/g_killspree.c
    game/g_main.c
    game/g_misc.c
    game/g_missile.c
    game/g_mover.c
    game/g_playerstore.c
    game/g_possession.c
    game/g_session.c
    game/g_spawn.c
    game/g_svcmds.c
    game/g_svcmds_ext.c
    game/g_syscalls.c
    game/g_target.c
    game/g_team.c
    game/g_trigger.c
    game/g_unlagged.c
    game/g_utils.c
    game/g_vote.c
    game/g_weapon.c
    qcommon/q_math.c
    qcommon/q_shared.c
)

# CGame source files
set(CGAME_SOURCES
    cgame/cg_challenges.c
    cgame/cg_compat_math.c
    cgame/cg_consolecmds.c
    cgame/cg_draw.c
    cgame/cg_drawtools.c
    cgame/cg_effects.c
    cgame/cg_ents.c
    cgame/cg_event.c
    cgame/cg_info.c
    cgame/cg_localents.c
    cgame/cg_main.c
    cgame/cg_marks.c
    cgame/cg_players.c
    cgame/cg_playerstate.c
    cgame/cg_predict.c
    cgame/cg_scoreboard.c
    cgame/cg_servercmds.c
    cgame/cg_snapshot.c
    cgame/cg_syscalls.c
    cgame/cg_unlagged.c
    cgame/cg_view.c
    cgame/cg_weapons.c
    game/bg_lib.c
    game/bg_misc.c
    game/bg_pmove.c
    game/bg_slidemove.c
    qcommon/q_math.c
    qcommon/q_shared.c
)

# UI (basegame / q3_ui style) source files
set(UI_SOURCES
    q3_ui/ui_addbots.c
    q3_ui/ui_atoms.c
    q3_ui/ui_cdkey.c
    q3_ui/ui_challenges.c
    q3_ui/ui_cinematics.c
    q3_ui/ui_confirm.c
    q3_ui/ui_connect.c
    q3_ui/ui_controls2.c
    q3_ui/ui_credits.c
    q3_ui/ui_demo2.c
    q3_ui/ui_display.c
    q3_ui/ui_firstconnect.c
    q3_ui/ui_gameinfo.c
    q3_ui/ui_ingame.c
    q3_ui/ui_loadconfig.c
    q3_ui/ui_main.c
    q3_ui/ui_menu.c
    q3_ui/ui_mfield.c
    q3_ui/ui_mods.c
    q3_ui/ui_network.c
    q3_ui/ui_password.c
    q3_ui/ui_playermodel.c
    q3_ui/ui_players.c
    q3_ui/ui_playersettings.c
    q3_ui/ui_preferences.c
    q3_ui/ui_qmenu.c
    q3_ui/ui_removebots.c
    q3_ui/ui_saveconfig.c
    q3_ui/ui_serverinfo.c
    q3_ui/ui_servers2.c
    q3_ui/ui_setup.c
    q3_ui/ui_sound.c
    q3_ui/ui_sparena.c
    q3_ui/ui_specifyserver.c
    q3_ui/ui_splevel.c
    q3_ui/ui_sppostgame.c
    q3_ui/ui_spskill.c
    q3_ui/ui_startserver.c
    q3_ui/ui_team.c
    q3_ui/ui_teamorders.c
    q3_ui/ui_video.c
    q3_ui/ui_votemenu.c
    q3_ui/ui_votemenu_custom.c
    q3_ui/ui_votemenu_fraglimit.c
    q3_ui/ui_votemenu_gametype.c
    q3_ui/ui_votemenu_kick.c
    q3_ui/ui_votemenu_map.c
    q3_ui/ui_votemenu_timelimit.c
    ui/ui_syscalls.c
    game/bg_lib.c
    game/bg_misc.c
    qcommon/q_math.c
    qcommon/q_shared.c
)

add_library(game_${ARCH_SUFFIX} SHARED ${GAME_SOURCES})
add_library(cgame_${ARCH_SUFFIX} SHARED ${CGAME_SOURCES})
add_library(ui_${ARCH_SUFFIX} SHARED ${UI_SOURCES})

target_compile_definitions(game_${ARCH_SUFFIX} PRIVATE QAGAME STANDALONE ARCH_STRING="${ARCH_SUFFIX}" BASEGAME="${OA_BASEGAME}")
target_compile_definitions(cgame_${ARCH_SUFFIX} PRIVATE CGAME STANDALONE ARCH_STRING="${ARCH_SUFFIX}" BASEGAME="${OA_BASEGAME}")
target_compile_definitions(ui_${ARCH_SUFFIX} PRIVATE UI STANDALONE ARCH_STRING="${ARCH_SUFFIX}" BASEGAME="${OA_BASEGAME}")

set_target_properties(game_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "game.${ARCH_SUFFIX}" PREFIX "")
set_target_properties(cgame_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "cgame.${ARCH_SUFFIX}" PREFIX "")
set_target_properties(ui_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "ui.${ARCH_SUFFIX}" PREFIX "")

file(MAKE_DIRECTORY "${OA_UI_OUTPUT_DIR}")
foreach(ui_text_file hud.txt hud2.txt ingame.txt menus.txt)
    configure_file("${OA_OUTPUT_DIR}/ui/${ui_text_file}" "${OA_UI_OUTPUT_DIR}/${ui_text_file}" COPYONLY)
endforeach()

if(BUILD_TESTING)
    add_executable(unit_cgame_compat_math
        tests/test_cgame_compat_math.c
        cgame/cg_compat_math.c)
    target_include_directories(unit_cgame_compat_math PRIVATE
        "${CMAKE_SOURCE_DIR}/cgame"
        "${CMAKE_SOURCE_DIR}/qcommon")
    target_compile_definitions(unit_cgame_compat_math PRIVATE
        ARCH_STRING="${ARCH_SUFFIX}"
        BASEGAME="${OA_BASEGAME}"
        STANDALONE)
    set_target_properties(unit_cgame_compat_math PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    if(UNIX)
        target_link_libraries(unit_cgame_compat_math PRIVATE m)
    endif()
    add_test(NAME unit_cgame_compat_math COMMAND $<TARGET_FILE:unit_cgame_compat_math>)
endif()

if(MSVC)
    target_compile_options(game_${ARCH_SUFFIX} PRIVATE /W4)
    target_compile_options(cgame_${ARCH_SUFFIX} PRIVATE /W4)
    target_compile_options(ui_${ARCH_SUFFIX} PRIVATE /W4)
    set_property(TARGET game_${ARCH_SUFFIX} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set_property(TARGET cgame_${ARCH_SUFFIX} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set_property(TARGET ui_${ARCH_SUFFIX} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    target_link_libraries(game_${ARCH_SUFFIX} m)
    target_link_libraries(cgame_${ARCH_SUFFIX} m)
    target_link_libraries(ui_${ARCH_SUFFIX} m)
endif()

target_include_directories(game_${ARCH_SUFFIX} PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(cgame_${ARCH_SUFFIX} PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(ui_${ARCH_SUFFIX} PRIVATE "${CMAKE_SOURCE_DIR}")
