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

int main() {
    plan(4);
    strToIntTest();
    pathCatTest();
    pathCatDealsWithLeadingSlashTest();
    pathCatDealsWithMultipleSlashTest();
    done_testing();
}