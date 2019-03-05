#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "common.h"

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

