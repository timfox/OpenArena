# OpenArena gamecode
![Build status](https://github.com/openarena/gamecode/actions/workflows/main.yml/badge.svg)

## Description ##
This is the game code part of OpenArena. In mod form it is referred as OpenArenaExpanded (OAX).

## Building ##

You need a C-compiler (tested with gcc and clang) and GNU make then just type
```
make
```
and the qvm-files will be build. Ready to be packed into a pk3-file.

See https://github.com/OpenArena/gamecode/wiki/Build-instruction for more details.

See http://openarena.wikia.com/wiki/OpenArena_eXpanded for alternative build options

## Extracting entities ##
It is possible to extract entity definition for use with GtkRadiant and NetRadiant like this:

```
cd code/game
./extract_entities.sh > openarena.def
```

## Links ##
Development documentation is located here: https://github.com/OpenArena/gamecode/wiki

The development board on the OpenArena forum: http://openarena.ws/board/index.php?board=30.0

In particular the Open Arena Expanded topic: http://openarena.ws/board/index.php?topic=1908.0

## License ##

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
