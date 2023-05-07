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
    is(result, expected, "paths match adding three fragments");
}

void pathCatDealsWithLeadingSlashTest() {
    // given
    char *path1 = "/path/to";
    char *path2 = "/dir";

    // when
    char *result = pathCat(path1, path2);
    char *expected = "/path/to/dir";

    // then
    is(result, expected, "paths match when second fragment has a leading slash");
}

void pathCatDealsWithMultipleSlashTest() {
    // given
    char *path1 = "/path/to/";
    char *path2 = "/dir";

    // when
    char *result = pathCat(path1, path2);
    char *expected = "/path/to/dir";

    // then
    is(result, expected, "paths match with slashes that need to be removed");
}

void canParseKV() {
    // given
    char data[] = "property:value";
    char *result[2];

    // when
    parseKV(data, result);

    // then
    is(result[0], "property", "key is 'property'");
    is(result[1], "value", "value is 'value'");
}

void canParseKVPairs() {
    // given
    char data[] = "abc:xyz\nzyx:cba";
    char *kvpairs[4];

    // when
    parseKVPairs(data, kvpairs);

    // then
    is(kvpairs[0], "abc", "key is 'abc'");
    is(kvpairs[1], "xyz", "value is 'xyz'");
    is(kvpairs[2], "zyx", "key is 'zyx'");
    is(kvpairs[3], "cba", "value is 'cba'");
}

int main() {
    plan(10);
    strToIntTest();
    pathCatTest();
    pathCatDealsWithLeadingSlashTest();
    pathCatDealsWithMultipleSlashTest();
    canParseKV();
    canParseKVPairs();
}