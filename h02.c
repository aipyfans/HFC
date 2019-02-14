#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h02.h"

/**
 * 展示存储器地址
 */
void getMemoryAddress() {
    int x = 8;
    // %p用来格式化地址(16进制)
    printf("x 保存在 %p\n", &x);
}

void go_south_east(int *lat, int *lon) {
    *lat = *lat - 1;
    *lon = *lon + 1;
}

/**
 * 指针实践
 */
void tryPointer() {
    int latitude = 32;
    int longitude = -64;
    go_south_east(&latitude, &longitude);
    printf("停！当前位置：[%i,%i]\n", latitude, longitude);
}

char tracks[][80] = {
        "I love you",
        "Dream tree",
        "Nice Girl"
};

void find_track(char search_for[]) {
    int i;
    for (i = 0; i < 5; i++) {
        if (strstr(tracks[i], search_for)) {
            printf("Track %i:'%s\n", i, tracks[i]);
        }
    }
}

/**
 * 根据关键词查找歌曲[2.5单元]
 */
void findSongByWord() {
    char search_for[80];
    printf("Search for:");
    fgets(search_for, 80, stdin);
    search_for[strlen(search_for) - 1] = '\0';
    find_track(search_for);
}

/**
 * 字符串反转：主要练习【指针算数运算】
 * @param s
 */
void print_reverse(char *s) {
    size_t len = strlen(s);
    char *t = s + len - 1;
    while (t >= s) {
        printf("%c", *t);
        t = t - 1;
    }
    puts("");
}