#include <istack.h>
#include <stdio.h>
#include <stdlib.h>

struct illnode {
    illnode* next;
    int val;
};

struct istack {
    illnode* top;
    int size;
};

istack* create_istack() {
    istack* s = malloc(sizeof(istack));
    s->top = NULL;
    s->size = 0;

    return s;
}

void ispush(istack* s, int val) {
    illnode* new_node = malloc(sizeof(illnode));
    new_node->next = s->top;
    new_node->val = val;

    s->top = new_node;
    s->size++;
}

int istop(istack* s) {
    if (s->size > 0) {
        return s->top->val;
    }
    else {
        printf("Error: istop called on empty stack.\n");
        exit(1);
    }
}

void ispop(istack* s) {
    if (s->size > 0) {
        illnode* next = s->top->next;

        free(s->top);
        s->top = next;

        s->size--;
    }
    else {
        printf("Error: ispop called on empty stack.\n");
        exit(1);
    }
}
