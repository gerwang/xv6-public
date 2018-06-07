//
// Created by gerw on 6/6/18.
//

#include "types.h"
#include "stat.h"
#include "user.h"

int dfs(int dep) {
    int unused[100];
    for (int i = 0; i < 100; i++) {
        unused[i] = rand();
    }
    printf(1, "now is at deep %d\n", dep);
    int res = dfs(dep + 1);
    return unused[23] + res;
}

int main(int argc, char *argv[]) {
    printf(1, "mycall return value is %d\n", mycall(233, "Hello world!"));
    printf(1, "Hello world! Sorry for being late!\n");
    printf(1, "result is %d\n", dfs(0));
    return 0;
}