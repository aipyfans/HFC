#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h07.h"

int NUM_ADS = 7;
char *ADS[] = {
        "William: SBM GSOH likes sports, TV, dining",
        "Matt: SWM NS likes art, movies, theater",
        "Luis: SLM ND likes books, theater, art",
        "Mike: DWM DS likes trucks, sports and bieber",
        "Peter: SAM likes chess, working out and art",
        "Josh: SJM likes sports, movies and theater",
        "Jed: DBM likes theater, books and dining"
};

int sports_no_bieber(char *s) {
    return strstr(s, "sports") && !strstr(s, "bieber");
}

int sports_or_workout(char *s) {
    return strstr(s, "sports") || strstr(s, "working out");
}

int ns_theacher(char *s) {
    return strstr(s, "NS") && strstr(s, "theater");
}

int arts_theater_or_dining(char *s) {
    return strstr(s, "arts") || strstr(s, "theater") || strstr(s, "dining");
}


void find(int (*match)(char *)) {
    int i;
    puts("Search results:");
    puts("----------------------------------");
    for (i = 0; i < NUM_ADS; i++) {
        if (match(ADS[i])) {
            printf("%s\n", ADS[i]);
        }
    }
    puts("----------------------------------");
}

void find_test() {
    find(sports_no_bieber);
    find(sports_or_workout);
    find(ns_theacher);
    find(arts_theater_or_dining);
}

/**
 * 升序排列整型得分
 * @param score_a
 * @param score_b
 * @return
 */
int compare_scores(const void *score_a, const void *score_b) {
    int a = *(int *) score_a;
    int b = *(int *) score_b;
    return a - b;
}

/**
 * 降序排列整型得分
 * @param score_a
 * @param score_b
 * @return
 */
int compare_scores_desc(const void *score_a, const void *score_b) {
    int a = *(int *) score_a;
    int b = *(int *) score_b;
    return b - a;
}

typedef struct {
    int width;
    int height;
} rectangle;

/**
 * 按面积从小到大排列据此那个
 * @param a
 * @param b
 * @return
 */
int compare_areas(const void *a, const void *b) {
    rectangle *ra = (rectangle *) a;
    rectangle *rb = (rectangle *) b;
    int area_a = (ra->width * ra->height);
    int area_b = (rb->width * rb->height);
    return area_a - area_b;
}

/**
 * 按面积从大到小排列据此那个
 * @param a
 * @param b
 * @return
 */
int compare_areas_desc(const void *a, const void *b) {
    return compare_areas(b, a);
}

int compare_names(const void *a, const void *b) {
    char **sa = (char **) a;
    char **sb = (char **) b;
    return strcmp(sa, sb);
}

void test_qsort() {
    int scores[] = {543, 323, 32, 554, 11, 3, 112};
    int i;
    qsort(scores, 7, sizeof(int), compare_scores_desc);
    puts("These are the scores in order:");
    for (i = 0; i < 7; ++i) {
        printf("Score = %i\n", scores[i]);
    }

    char *names[] = {"Karen", "Mark", "Brett", "Molly"};
    qsort(names, 4, sizeof(char *), compare_names);
    puts("These are the names in order:");
    for (i = 0; i < 4; ++i) {
        printf("Name = %s\n", names[i]);
    }
}