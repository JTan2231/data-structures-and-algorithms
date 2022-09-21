#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryNode BinaryNode;
typedef struct BinaryStack BinaryStack;
typedef struct BinaryLLNode BinaryLLNode;
typedef struct BinaryQueue BinaryQueue;

struct BinaryNode {
    int val;
    BinaryNode* left;
    BinaryNode* right;
};

// move extraneous data structures to another file
struct BinaryStack {
    BinaryNode** stack;
    int size;
    int capacity;
};

struct BinaryLLNode {
    BinaryLLNode* prev;
    BinaryLLNode* next;
    BinaryNode* val;
};

struct BinaryQueue {
    BinaryLLNode* front;
    BinaryLLNode* back;
    int size;
};


BinaryStack* createBinaryStack() {
    BinaryStack* stack = malloc(sizeof(BinaryStack));

    stack->size = 0;
    stack->capacity = 1;

    return stack;
}

void spush(BinaryStack* stack, BinaryNode* node) {
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

int spop(BinaryStack* stack) {
    if (stack->size > 0) {
        stack->stack[stack->size-1] = NULL;
        stack->size--;
    }
    else {
        return -1;
    }

    return 0;
}

BinaryNode* stop(BinaryStack* stack) {
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

BinaryQueue* createBinaryQueue() {
    BinaryQueue* queue = malloc(sizeof(BinaryQueue));
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;

    return queue;
}

void qpush(BinaryQueue* queue, BinaryNode* node) {
    BinaryLLNode* entry = malloc(sizeof(BinaryLLNode));
    entry->val = node;

    if (queue->size > 0) {
        entry->next = queue->back;
        queue->back->prev = entry;
    }
    else {
        queue->front = entry;
    }

    queue->back = entry;
    queue->size++;
}

int qpop(BinaryQueue* queue) {
    if (queue->size > 0) {
        BinaryLLNode* temp = queue->front->prev;

        free(queue->front);
        queue->front = temp;

        if (temp) {
            temp->next = NULL;
        }

        queue->size--;

        return 0;
    }
    else {
        return -1;
    }
}

BinaryNode* qfront(BinaryQueue* queue) {
    if (queue->size > 0) {
        return queue->front->val;
    }
    else {
        return NULL;
    }
}

void freeQueue(BinaryQueue* queue) {
    BinaryLLNode* next = queue->front;
    while (next) {
        free(next);
        next = next->next;
    }

    free(queue);
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
    spush(s, root);

    BinaryNode* current;
    while (s->size > 0) {
        current = stop(s);
        spop(s);

        if (current->val == target) {
            return current;
        }

        if (current->left) {
            spush(s, current->left);
        }

        if (current->right) {
            spush(s, current->right);
        }
    }

    freeStack(s);

    return NULL;
}

char numOrPad(char c) {
    if (c >= '0' && c <= '9') {
        return c;
    }
    else {
        return '#';
    }
}

// uses bfs
void serializeBinaryTree(char* filename, BinaryNode* root) {
    FILE* fp = fopen(filename, "w");

    if (!fp) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    BinaryQueue* q = createBinaryQueue();
    qpush(q, root);

    BinaryNode* current;
    while (q->size > 0) {
        int n = q->size;
        for (int i = 0; i < n; i++) {
            current = qfront(q);
            qpop(q);

            char c[11] = "##########";
            snprintf(c, 11, "%d", current->val);

            char l[11] = "n#########";
            char r[11] = "n#########";
            if (current->left) {
                qpush(q, current->left);
                snprintf(l, 11, "%d", current->left->val);
            }

            if (current->right) {
                qpush(q, current->right);
                snprintf(r, 11, "%d", current->right->val);
            }

            char node_trio[34];
            for (int i = 0; i < 10; i++) {
                node_trio[i] = numOrPad(c[i]);
            }

            node_trio[10] = ';';
            for (int i = 11; i < 21; i++) {
                node_trio[i] = numOrPad(l[i-11]);
            }

            node_trio[21] = ';';
            for (int i = 22; i < 32; i++) {
                node_trio[i] = numOrPad(r[i-22]);
            }

            node_trio[32] = ';';
            node_trio[33] = '\0';

            fprintf(fp, "%s", node_trio);
        }

        fprintf(fp, "\n");
    }

    freeQueue(q);
    fclose(fp);

    printf("Binary tree saved to %s\n", filename);
}

void getNodeVal(char* line, char* node, int num, int len) {
    for (int i = num*len; i < (num*len) + len; i++) {
        node[i-(num*len)] = line[i];
    }

    node[len-1] = '\0';
}

BinaryNode* deserializeBinaryTree(char* filename) {
    FILE* fp = fopen(filename, "r");

    if (!fp) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    BinaryNode* root = NULL;
    BinaryQueue* q = createBinaryQueue();

    char* line = NULL;
    size_t n = 0;
    ssize_t read;

    unsigned int len;
    while ((read = getline(&line, &n, fp)) != -1) {
        len = (int)(ceil((read - 1) / 11.)); // number of nodes read into line

        // NOTE: len is guaranteed to be at least 3
        int i = 0;
        if (!root) {
            char node_val[11];
            getNodeVal(line, node_val, i, 11);

            if (node_val[0] != '#') {
                char* end;
                root = createNode(strtol(node_val, &end, 10));
            }
            else {
                printf("Error: malformed input.");
                exit(1);
            }

            qpush(q, root);
        }

        while (i < len) {
            BinaryNode* parent;
            if (i % 3 == 0) {
                parent = qfront(q);
                qpop(q);

                i++;
                continue;
            }

            char node_val[11];
            getNodeVal(line, node_val, i, 11);

            char* end;
            if (node_val[0] != '#') {
                if ((i-1) % 3 == 0) {
                    parent->left = createNode(strtol(node_val, &end, 10));
                    qpush(q, parent->left);
                }
                else {
                    parent->right = createNode(strtol(node_val, &end, 10));
                    qpush(q, parent->right);
                }
            }

            i++;
        }
    }

    return root;
}

void printBinaryTree(BinaryNode* root) {
    BinaryQueue* q = createBinaryQueue();
    qpush(q, root);

    BinaryNode* current;
    while (q->size > 0) {
        int n = q->size;
        for (int i = 0; i < n; i++) {
            current = qfront(q);
            qpop(q);

            char* p = malloc(sizeof(char) * (int)log10(current->val));
            char* l;
            char* r;

            sprintf(p, "%d", current->val);

            if (current->left) {
                qpush(q, current->left);

                l = malloc(sizeof(char) * (int)log10(current->left->val));
                sprintf(l, "%d", current->left->val);
            }
            else {
                l = "n";
            }

            if (current->right) {
                qpush(q, current->right);

                r = malloc(sizeof(char) * (int)log10(current->right->val));
                sprintf(r, "%d", current->right->val);
            }
            else {
                r = "n";
            }

            printf("%s, %s, %s; ", p, l, r);
        }

        printf("\n");
    }
}

int main() {
    BinaryNode* node = createNode(5);
    node->left = createNode(10);
    node->right = createNode(15);

    node->left->left = createNode(22);
    node->right->left = createNode(36);
    node->left->left->right = createNode(43);

    printBinaryTree(node);

    char filename[] = "tree.btr";

    serializeBinaryTree(filename, node);

    printf("DESERIALIZED:\n");
    BinaryNode* root = deserializeBinaryTree(filename);
    printBinaryTree(root);

    return 0;
}
