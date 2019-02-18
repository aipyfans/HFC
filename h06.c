#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h06.h"

typedef struct island {
    char *name;
    char *opens;
    char *closes;
    struct island *next;
} island;

void display(island *start) {
    island *i = start;
    for (; i != NULL; i = i->next)
        printf("Name:%s open:%s-%s\n", i->name, i->opens, i->closes);
}

island *create(char *name) {
    island *i = malloc(sizeof(island));
    i->name = strdup(name);
    i->opens = "09:00";
    i->closes = "17:00";
    i->next = NULL;
    return i;
}

void release(island *start) {
    island *i = start;
    island *next = NULL;
    for (; i != NULL; i = next) {
        next = i->next;
        free(i->name);
        free(i);
    }
}

void travel_island() {
    island amity = {"Amity", "09:00", "17:00", NULL};
    island craggy = {"Craggy", "09:00", "17:00", NULL};
    island isla_nublar = {"Isla Nublar", "09:00", "17:00", NULL};
    island shutter = {"Shutter", "09:00", "17:00", NULL};

    amity.next = &craggy;
    craggy.next = &isla_nublar;
    isla_nublar.next = &shutter;

    island skull = {"Skull", "09:00", "17:00", NULL};
    isla_nublar.next = &skull;
    skull.next = &shutter;

    display(&amity);

}

/**
 * /HFC < island.txt
 */
void travel_island2() {
    island *start = NULL;
    island *i = NULL;
    island *next = NULL;
    char name[80];

    for (; fgets(name, 80, stdin) != NULL; i = next) {
        next = create(name);
        if (start == NULL)
            start = next;
        if (i != NULL)
            i->next = next;
    }
    display(start);
    release(start);
}


typedef struct node {
    char *question;
    struct node *no;
    struct node *yes;
} node;

int yes_no(char *question) {
    char answer[3];
    printf("%s? (y/n): ", question);
    fgets(answer, 3, stdin);
    return answer[0] == 'y';
}

node *createNode(char *question) {
    node *n = malloc(sizeof(node));
    n->question = strdup(question);
    n->no = NULL;
    n->yes = NULL;
    return n;
}

void releaseNode(node *n) {
    if (n) {
        if (n->no)
            releaseNode(n->no);
        if (n->yes)
            releaseNode(n->yes);
        if (n->question)
            releaseNode(n->question);
        free(n);
    }
}

int spies() {

    char question[80];
    char suspect[20];
    node *start_node = createNode("Does suspect have a mustache");
    start_node->no = createNode("Loretta Barnsworth");
    start_node->yes = createNode("Vinny the Spoon");

    node *current;
    do {
        current = start_node;
        while (1) {
            if (yes_no(current->question)) {
                if (current->yes) {
                    current = current->yes;
                } else {
                    printf("SUSPECT IDENTIFIED\n");
                    break;
                }
            } else if (current->no) {
                current = current->no;
            } else {
                /*Make the yes-node the new suspect name*/
                printf("Who's the suspect?");
                fgets(suspect, 20, stdin);
                node *yes_node = createNode(suspect);
                current->yes = yes_node;

                /*Make the no-node a copy of this question*/
                node *no_node = createNode(current->question);
                current->no = no_node;

                /*Then replace this question with the new question*/
                printf("Give me a question that is TRUE for %s but not for %s?", suspect, current->question);
                fgets(question, 80, stdin);

                // 把上一个问题的空间给释放掉,否则造成存储器泄露
                free(current->question);

                current->question = strdup(question);
                break;
            }
        }
    } while (yes_no("Run again"));

    releaseNode(start_node);
    return 0;
}