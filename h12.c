#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "common.h"

#include "h12.h"

void *does_not(void *a) {
    int i = 0;
    for (i = 0; i < 5; ++i) {
        sleep(1);
        puts("Does not!");
    }
    return NULL;
}


void *does_too(void *a) {
    int i = 0;
    for (i = 0; i < 5; ++i) {
        sleep(1);
        puts("Does too!");
    }
    return NULL;
}


void thread_one() {
    pthread_t t0;
    pthread_t t1;
    if (pthread_create(&t0, NULL, does_not, NULL) == -1)
        error("Can't create thread t0");
    if (pthread_create(&t1, NULL, does_too, NULL) == -1)
        error("Can't create thread t1");

    void *result;
    if (pthread_join(t0, &result) == -1)
        error("Can't recycle thread t0");
    if (pthread_join(t1, &result) == -1)
        error("Can't recycle thread t1");
}

int beers = 2000000;

pthread_mutex_t beers_lock = PTHREAD_MUTEX_INITIALIZER;

void *drink_lots(void *a) {
    int i;
    pthread_mutex_lock(&beers_lock);
    for (i = 0; i < 100000; i++) {
        beers = beers - 1;
    }
    pthread_mutex_unlock(&beers_lock);
    printf("beers = %i\n", beers);
    return NULL;
}

void *drink_lots2(void *a) {
    int i;
    for (i = 0; i < 100000; i++) {
        pthread_mutex_lock(&beers_lock);
        beers = beers - 1;
        pthread_mutex_unlock(&beers_lock);
    }
    printf("beers = %i\n", beers);
    return NULL;
}

void thread_lock() {
    pthread_t threads[20];
    printf("%i bottles of beer on the wall\n%i bottles of beer\n", beers, beers);
    for (int t = 0; t < 20; t++) {
        pthread_create(&threads[t], NULL, drink_lots2, NULL);
    }
    void *result;
    for (int t = 0; t < 20; t++) {
        pthread_join(threads[t], &result);
    }
    printf("There are now %i bottles of beer on the wall\n", beers);
}