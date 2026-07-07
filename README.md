# OpenArena-TF
![Build status](https://github.com/timfox/OpenArena/actions/workflows/main.yml/badge.svg)

## Description ##
Game code for OpenArena refactored to run on idTech3-TF.

## Native build ##
Build native `game`, `cgame`, and `ui` modules for `timfox/idtech3` with:

```sh
cmake -S gamesrc -B build-native -DENGINE_ROOT=/path/to/idtech3
cmake --build build-native -j
```

Artifacts are written to `vm/` as:

```text
vm/game.<arch>.so
vm/cgame.<arch>.so
vm/ui.<arch>.so
```

`ENGINE_ROOT` is optional when this repository already lives inside the
engine's mod tree. The CMake build detects both classic `src/...` layouts and
the `timfox/idtech3` `engine/core` plus `runtime/...` layout.
