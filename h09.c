#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "h09.h"

char *now() {
    time_t t;
    time(&t);
    return asctime(localtime(&t));
}

void test_system() {
    char comment[80];
    char cmd[120];

    fgets(comment, 80, stdin);
    sprintf(cmd, "echo '%s %s' >> reports.log", comment, now());
    system(cmd);
}

int test_exec() {
    if (execl("/sbin/ifconfig", "/sbin/ifconfig", NULL) == -1) {

        if (execl("ifconfig", "ifconfig", NULL) == -1) {

            fprintf(stderr, "Cannot run ipconfig: %s", strerror(errno));
            return 1;
        }
    }
    return 0;
}
