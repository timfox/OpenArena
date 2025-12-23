cmake_minimum_required(VERSION 3.31)
project(openarena_game_modules LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Put artifacts next to the mod so tooling can copy them easily
set(OA_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/..")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OA_OUTPUT_DIR}/vm")

# Engine headers live three levels up
get_filename_component(ENGINE_ROOT "${CMAKE_SOURCE_DIR}/../../.." ABSOLUTE)
set(ENGINE_INCLUDE_DIRS
    "${ENGINE_ROOT}/src/qcommon"
    "${ENGINE_ROOT}/src/game"
    "${ENGINE_ROOT}/src/cgame"
    "${ENGINE_ROOT}/src/ui"
    "${ENGINE_ROOT}/src/renderercommon"
    "${ENGINE_ROOT}/src/client"
)

message(STATUS "ENGINE_ROOT: ${ENGINE_ROOT}")
message(STATUS "Library output: ${OA_OUTPUT_DIR}/vm")

# Local headers MUST come first to avoid using engine's standard headers
include_directories("${CMAKE_SOURCE_DIR}/game")
include_directories("${CMAKE_SOURCE_DIR}/cgame")
include_directories("${CMAKE_SOURCE_DIR}/ui")
include_directories("${CMAKE_SOURCE_DIR}/q3_ui")
include_directories("${CMAKE_SOURCE_DIR}/qcommon")
include_directories(${ENGINE_INCLUDE_DIRS})

add_compile_options(-fPIC -fvisibility=hidden -Wall -Wextra -Wno-unused-parameter)

if(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(ARCH_SUFFIX "x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86")
        set(ARCH_SUFFIX "i386")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(ARCH_SUFFIX "arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        set(ARCH_SUFFIX "arm")
    endif()
elseif(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_SUFFIX "x64")
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
    cgame/cg_newdraw.c
    game/bg_lib.c
    game/bg_misc.c
    game/bg_pmove.c
    game/bg_slidemove.c
    qcommon/q_math.c
    qcommon/q_shared.c
    ui/ui_shared.c
)

# UI (MissionPack style) source files
set(UI_SOURCES
    ui/ui_main.c
    ui/ui_atoms.c
    ui/ui_gameinfo.c
    ui/ui_players.c
    ui/ui_shared.c
    ui/ui_syscalls.c
    game/bg_lib.c
    game/bg_misc.c
    qcommon/q_math.c
    qcommon/q_shared.c
)

add_library(game_${ARCH_SUFFIX} SHARED ${GAME_SOURCES})
add_library(cgame_${ARCH_SUFFIX} SHARED ${CGAME_SOURCES})
add_library(ui_${ARCH_SUFFIX} SHARED ${UI_SOURCES})

target_compile_definitions(game_${ARCH_SUFFIX} PRIVATE QAGAME MISSIONPACK ARCH_STRING="${ARCH_SUFFIX}")
target_compile_definitions(cgame_${ARCH_SUFFIX} PRIVATE CGAME MISSIONPACK ARCH_STRING="${ARCH_SUFFIX}")
target_compile_definitions(ui_${ARCH_SUFFIX} PRIVATE UI MISSIONPACK ARCH_STRING="${ARCH_SUFFIX}")

set_target_properties(game_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "game${ARCH_SUFFIX}" PREFIX "")
set_target_properties(cgame_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "cgame${ARCH_SUFFIX}" PREFIX "")
set_target_properties(ui_${ARCH_SUFFIX} PROPERTIES OUTPUT_NAME "ui${ARCH_SUFFIX}" PREFIX "")

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
