cmake_minimum_required (VERSION 3.6)
project (cjrpgengine)
include(ExternalProject)
set(CMAKE_BUILD_TYPE FILEPATH)
set(EXECUTABLE_OUTPUT_PATH ./build CACHE PATH "Build directory" FORCE)
set(CMAKE_C_FLAGS "-Isrc")

# versions
set(RAYLIB_VERSION 4.2.0)
set(LIBTAP_VERSION 0.1.0)
set(LIBYAML_VERSION 1.4.1)

# add raylib
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

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
target_link_libraries(tests raylib)
target_link_libraries(tests tap)
target_link_libraries(tests ${LIBXML2_LIBRARIES})
target_link_libraries(tests cyaml)

# define main executable
add_executable(cjrpgengine src/main.c)
target_link_libraries(cjrpgengine raylib)
target_link_libraries(cjrpgengine ${LIBXML2_LIBRARIES})
target_link_libraries(cjrpgengine cyaml)
