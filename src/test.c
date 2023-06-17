#include "header.h"
#include "tap.h"

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

int main() {
    plan(1);
    strToIntTest();
    done_testing();
}