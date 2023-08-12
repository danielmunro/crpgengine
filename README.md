# RPGEngine

RPGEngine plans to provide all the needed facilities for users to script their
own RPG games. RPGEngine integrates with the open source [Tiled map editor](https://www.mapeditor.org/),
which provides a rich environment for building worlds.

The scripting language of RPGEngine is YAML, so if you know YAML there is no
new syntax to learn.

## Running RPGEngine

Clone this repository:

```
git clone git@github.com:danielmunro/cjrpgengine.git
```

Make the project:

```
make
```

Run the provided example game:

```
./build/cjrpgengine -i examples/simple_demo
```

## Runtime Arguments

Required:

`-i` -- index directory to load game from. Example game argument is `examples/simple_demo`.

Debug arguments:

`-c` -- show collisions. Comma separated list, containing: 'player', 'objects', and/or 'warps'. For example, to show all three: '-c player,objects,warps'

`-d` -- dump the state of the loaded game and quit.

`-l` -- specify the log level, one of [debug, info, warn, error]. Default is `info`.

`-s` -- override the game config and start at specified scene index.

`-e` -- exit after loading the game. Useful for debugging startup problems.

`-v` -- load a specific save file.

`-n` -- force a new game.

`-m` -- output max memory usage each second.

`-ps` -- purge saves (careful: saves are unrecoverable).

## Todos

* Finish fight implementation
* Save/load
