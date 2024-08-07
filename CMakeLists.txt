cmake_minimum_required (VERSION 3.6)
project (crpgengine)
include(ExternalProject)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ./build CACHE PATH "Build directory" FORCE)
set(CMAKE_C_FLAGS "-Isrc -fprofile-instr-generate -fcoverage-mapping")

# versions
set(RAYLIB_VERSION 5ede47618bd9f9a440af648da1b4817e51644994)
set(LIBTAP_VERSION 0.1.0)
set(LIBYAML_VERSION 1.4.1)

# add raylib
include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/${RAYLIB_VERSION}.tar.gz
)
FetchContent_MakeAvailable(raylib)

# add libxml2
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

# add libtap
ExternalProject_Add(LibTap
        URL https://github.com/zorgnax/libtap/archive/refs/tags/${LIBTAP_VERSION}.tar.gz
        PREFIX _deps
        INSTALL_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1)
link_directories(_deps/src/LibTap)
include_directories(_deps/src/LibTap)

# add libyaml
ExternalProject_Add(LibYaml
        URL https://github.com/tlsa/libcyaml/archive/refs/tags/v${LIBYAML_VERSION}.tar.gz
        PREFIX _deps
        INSTALL_COMMAND make install DESTDIR=release
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1)
link_directories(_deps/src/LibYaml/release/usr/local/lib)
include_directories(_deps/src/LibYaml/release/usr/local/include)

# add project source
include_directories(.)

# define tests executable
add_executable(tests src/test.c)
target_link_libraries(tests raylib tap cyaml ${LIBXML2_LIBRARIES})

# define main executable
add_executable(crpgengine src/main.c)
target_link_libraries(crpgengine raylib cyaml ${LIBXML2_LIBRARIES})
