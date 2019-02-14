#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h03.h"

/**
 * 默认的标准输入和输出
 */
void map() {
    float latitude;
    float longitude;

    char info[80];
    int started = 0;

    puts("data=");
    while (scanf("%f,%f,%79[^\n]", &latitude, &longitude, info) == 3) {
        if (started)
            printf(",\n");
        else
            started = 1;
        printf("{latitude:%f,longitude:%f,info:'%s'}", latitude, longitude, info);
    }
    puts("\n]");
}


/**
 * 默认的标准输入和输出以及错误
 */
int map_stderr() {
    float latitude;
    float longitude;

    char info[80];
    int started = 0;

    puts("data=");
    while (scanf("%f,%f,%79[^\n]", &latitude, &longitude, info) == 3) {
        if (started)
            printf(",\n");
        else
            started = 1;
        if ((latitude < -90.0) || (latitude > 90.0)) {
            fprintf(stderr, "Invalid latitude:%f\n", latitude);
            return 2;
        }
        if ((longitude < -180.0) || (longitude > 180.0)) {
            fprintf(stderr, "Invalid longitude:%f\n", longitude);
            return 2;
        }
        printf("{latitude:%f,longitude:%f,info:'%s'}", latitude, longitude, info);
    }
    puts("\n]");
    return 0;
}


void map_section() {
    float latitude;
    float longitude;
    char info[80];
    while (scanf("%f,%f,%79[^\n]", &latitude, &longitude, info) == 3) {
        if ((latitude > 26) && (latitude < 34)) {
            if ((longitude > -76) && (longitude < -64)) {
                printf("%f,%f,%s\n", latitude, longitude, info);
            }
        }
    }
}

/**
 * 实践：文件数据流
 * /Users/william/Dream/WorkSpace/C2pp/HFC/cmake-build-debug/HFC UFO g_ufo.csv Elvis g_elvis.csv g_other.csv
 * @param argc
 * @param argv
 * @return
 */
int map_type(int argc, char *argv[]) {
    char line[80];
    if (argc != 6) {
        fprintf(stderr, "you need to give 5 arguments\n");
        return 1;
    }

    FILE *in = fopen("gpstype.csv", "r");

    FILE *file1 = fopen(argv[2], "w");
    FILE *file2 = fopen(argv[4], "w");
    FILE *file3 = fopen(argv[5], "w");

    while (fscanf(in, "%79[^\n]\n", line) == 1) {
        if (strstr(line, argv[1]))
            fprintf(file1, "%s\n", line);
        else if (strstr(line, argv[3]))
            fprintf(file2, "%s\n", line);
        else
            fprintf(file3, "%s\n", line);
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);
    return 0;
}



// 重定向标准输入
//   /Users/william/Dream/WorkSpace/C2pp/HFC/cmake-build-debug/HFC < /Users/william/Dream/WorkSpace/C2pp/HFC/gpsdata.csv

// 重定向 - 标准输入 & 标准输出
//   /Users/william/Dream/WorkSpace/C2pp/HFC/cmake-build-debug/HFC < /Users/william/Dream/WorkSpace/C2pp/HFC/gpsdata.csv > /Users/william/Dream/WorkSpace/C2pp/HFC/gpsdata.json

// 重定向 - 标准输入 & 标准输出 & 管道
//   (/Users/william/Dream/WorkSpace/C2pp/HFC/cmake-build-debug/HFCO | /Users/william/Dream/WorkSpace/C2pp/HFC/cmake-build-debug/HFC) < /Users/william/Dream/WorkSpace/C2pp/HFC/gpsdata.csv > /Users/william/Dream/WorkSpace/C2pp/HFC/gpsdata.json

