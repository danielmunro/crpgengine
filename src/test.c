#include "header.h"
#include "tap.h"

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

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

void pathCatDealsWithLeadingSlashTest() {
    // given
    char *path1 = "/path/to";
    char *path2 = "/dir";

    // when
    char *result = pathCat(path1, path2);
    char *expected = "/path/to/dir";

    // then
    is(result, expected, "paths should match");
}

void pathCatDealsWithMultipleSlashTest() {
    // given
    char *path1 = "/path/to/";
    char *path2 = "/dir";

    // when
    char *result = pathCat(path1, path2);
    char *expected = "/path/to/dir";

    // then
    is(result, expected, "paths should match");
}

void canParseKV() {
    // given
    char data[] = "property:value";
    char *result[2];

    // when
    parseKV(data, result);

    // then
    is(result[0], "property", "key should be 'property'");
    is(result[1], "value", "value should be 'value'");
}

int main() {
    plan(5);
    strToIntTest();
    pathCatTest();
    pathCatDealsWithLeadingSlashTest();
    pathCatDealsWithMultipleSlashTest();
    canParseKV();
}