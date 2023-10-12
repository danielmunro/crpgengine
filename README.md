# RPGEngine

RPGEngine plans to provide all the needed facilities for users to script their
own RPG games. RPGEngine integrates with the open source [Tiled map editor](https://www.mapeditor.org/),
which provides a rich environment for building worlds.

The scripting language of RPGEngine is YAML, so if you know YAML there is no
new syntax to learn.

## Running RPGEngine

TODO: document host system dependencies.

Clone this repository:

```
git clone git@github.com:danielmunro/cjrpgengine.git
```

Run cmake:

```
cmake .
```

Make the project:

```
make
```

Run the provided example game:

```
./build/cjrpgengine
```

## Runtime Configuration

Check out `config.yaml` to see configuration options.
