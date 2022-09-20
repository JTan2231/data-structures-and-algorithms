#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryNode BinaryNode;
typedef struct BinaryStack BinaryStack;

struct BinaryNode {
    int val;
    BinaryNode* left;
    BinaryNode* right;
};

struct BinaryStack {
    BinaryNode** stack;
    int size;
    int capacity;
};

BinaryStack* createBinaryStack() {
    BinaryStack* stack = malloc(sizeof(BinaryStack));

    stack->size = 0;
    stack->capacity = 1;

    return stack;
}

void push(BinaryStack* stack, BinaryNode* node) {
    if (stack->size + 1 == stack->capacity) {
        BinaryNode** new_alloc = malloc(2 * stack->capacity * sizeof(BinaryNode));

        for (int i = 0; i < stack->size; i++) {
            new_alloc[i] = stack->stack[i];
        }

        free(stack->stack);

        stack->stack = new_alloc;
        stack->capacity *= 2;
    }

    stack->stack[stack->size] = node;
    stack->size++;
}

int pop(BinaryStack* stack) {
    if (stack->size > 0) {
        stack->stack[stack->size-1] = NULL;
        stack->size--;
    }
    else {
        return -1;
    }

    return 0;
}

BinaryNode* top(BinaryStack* stack) {
    if (stack->size > 0) {
        return stack->stack[stack->size-1];
    }
    else {
        return NULL;
    }
}

void freeStack(BinaryStack* stack) {
    free(stack->stack);
    free(stack);
}

BinaryNode* createNode(int value) {
    BinaryNode* new_node = malloc(sizeof(BinaryNode));

    new_node->val = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

BinaryNode* dfs_preorder(BinaryNode* root, int target) {
    BinaryStack* s = createBinaryStack();
    push(s, root);

    BinaryNode* current;
    while (s->size > 0) {
        current = top(s);
        pop(s);

        if (current->val == target) {
            return current;
        }

        if (current->left) {
            push(s, current->left);
        }

        if (current->right) {
            push(s, current->right);
        }
    }

    freeStack(s);

    return NULL;
}

int main() {
    BinaryNode* node = createNode(5);
    node->left = createNode(10);
    node->right = createNode(15);

    node->left->left = createNode(22);
    node->left->left->right = createNode(43);

    BinaryNode* target = node->left->left->right;
    printf("Target node: %p\n", target);

    BinaryNode* found = dfs_preorder(node, target->val);
    printf("Found node: %p\n", found);

    return 0;
}
