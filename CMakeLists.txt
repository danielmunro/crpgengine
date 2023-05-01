cmake_minimum_required (VERSION 3.6)
project (cjrpgengine)

set(RAYLIB_VERSION 4.2.0)
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

find_package(LibXml2 REQUIRED)

FetchContent_Declare(
        tap
        GIT_REPOSITORY https://github.com/zorgnax/libtap.git
        GIT_TAG        0.1.0
)
link_directories(/usr/local/lib)
add_executable(tests src/test.c)
include_directories(/usr/local/include)
target_link_libraries(tests raylib)
target_link_libraries(tests tap)
target_link_libraries(tests ${LIBXML2_LIBRARIES})

add_executable(cjrpgengine src/main.c)
set(EXECUTABLE_OUTPUT_PATH ./build CACHE PATH "Build directory" FORCE)
include_directories(.)
include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(cjrpgengine raylib)
target_link_libraries(cjrpgengine ${LIBXML2_LIBRARIES})
