cmake_minimum_required (VERSION 3.6)
project (crpgengine)
include(ExternalProject)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ./build CACHE PATH "Build directory" FORCE)
set(CMAKE_C_FLAGS "-Isrc")

# versions
set(RAYLIB_VERSION 4.2.0)
set(LIBTAP_VERSION b53e4ef5257f80e881762b6143834d8aae29da1a)
set(LIBYAML_VERSION e2bea44c472719daff095541b22b038fa54c274b)

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

# add libyaml
ExternalProject_Add(libcyaml
        GIT_REPOSITORY https://github.com/tlsa/libcyaml.git
        GIT_TAG ${LIBYAML_VERSION}
        PREFIX _deps
        INSTALL_COMMAND make install DESTDIR=release
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1)

include_directories(_deps/src/libcyaml/release/usr/local/include)
link_directories(_deps/src/libcyaml/release/usr/local/lib)

# add libyaml
ExternalProject_Add(libtap
        GIT_REPOSITORY https://github.com/zorgnax/libtap.git
        GIT_TAG ${LIBTAP_VERSION}
        PREFIX _deps
        INSTALL_COMMAND make install DESTDIR=release
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1)

include_directories(_deps/src/libtap/release/usr/local/include)
link_directories(_deps/src/libtap/release/usr/local/lib)

#add_library(libcyaml STATIC IMPORTED GLOBAL)
#set_properties(libcyaml PROPERTIES
#        IMPORTED_LOCATION "_deps/libcyaml-src${CMAKE_STATIC_LIBRARY_PREFIX}")
#target_include_directories(libcyaml INTERFACE "_deps/include")

#link_directories(_deps/src/LibYaml/release/usr/local/lib)
#include_directories(_deps/src/LibYaml/release/usr/local/include)

# add project source
#include_directories(.)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

file(COPY _deps/src/libcyaml/release/usr/local/lib/libcyaml.1.dylib DESTINATION . FOLLOW_SYMLINK_CHAIN)
file(COPY _deps/src/libtap/release/usr/local/lib/libtap.so DESTINATION . FOLLOW_SYMLINK_CHAIN)

# define tests executable
add_executable(tests src/test.c)
target_link_libraries(tests tap)
target_link_libraries(tests raylib)
target_link_libraries(tests cyaml)
target_link_libraries(tests ${LIBXML2_LIBRARIES})

# define main executable
add_executable(crpgengine src/main.c)
target_link_libraries(crpgengine cyaml)
target_link_libraries(crpgengine raylib)
target_link_libraries(crpgengine ${LIBXML2_LIBRARIES})
