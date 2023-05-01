#include "header.h"
#include "tap.h"

void pathCatTest() {
    // given
    char *path1 = "/path/to/";
    char *path2 = "dir";
    char *path3 = "./nested";
    char *expected = "/path/to/dir/./nested";

    // when
    char *part = pathCat(path1, path2);
    char *result = pathCat(part, path3);

    // then
    is(result, expected, "paths should match");
}

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

int main() {
    plan(2);
    pathCatTest();
    strToIntTest();
}