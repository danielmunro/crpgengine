# RPGEngine

RPGEngine plans to provide all the needed facilities for users to script their
own RPG games. RPGEngine integrates with the open source [Tiled map editor](https://www.mapeditor.org/),
which provides a rich environment for building worlds.

The scripting language of RPGEngine is YAML, so if you know YAML there is no
new syntax to learn.

## Running RPGEngine

1. Clone this repository and `cd` to it:

    ```
    git clone git@github.com:danielmunro/cjrpgengine.git
    cd crpgengine
    ```

2. Make the project:

    ```
    cmake .
    make
    ```

3. Export the following dynamic lib fallback paths:

    ```
    export DYLD_FALLBACK_LIBRARY_PATH=$PWD/_deps/src/LibTap:$PWD/_deps/src/LibYaml/release/usr/local/lib
    ```

4. Run the provided example game:
    
    ```
    ./build/crpgengine
    ```
   
    Tests can also be run:

    ```
    ./build/tests
    ```

## Runtime Configuration

Check out `config.yaml` to see configuration options.
