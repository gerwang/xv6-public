//
// Created by gerw on 6/7/18.
//


#include "types.h"
#include "defs.h"

int sys_mycall(void) {
    int myarg;
    char *mystr;
    if (argint(0, &myarg) < 0) {
        return -1;
    }
    if (argstr(1, &mystr) < 0) {
        return -2;
    }
    cprintf("I've read %d and %s!\n", myarg, mystr);
    return 0;
}