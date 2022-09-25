#ifndef BINARY_TREE
#define BINARY_TREE

typedef struct BinaryNode BinaryNode;
typedef struct BinaryStack BinaryStack;
typedef struct BinaryLLNode BinaryLLNode;
typedef struct BinaryQueue BinaryQueue;

BinaryStack* createBinaryStack();
void spush(BinaryStack* stack, BinaryNode* node);
int spop(BinaryStack* stack);
BinaryNode* stop(BinaryStack* stack);
void freeStack(BinaryStack* stack);

BinaryQueue* createBinaryQueue();
BinaryLLNode* createBinaryLLNode(BinaryNode* val);
void qpush(BinaryQueue* queue, BinaryNode* node);
int qpop(BinaryQueue* queue);
BinaryNode* qfront(BinaryQueue* queue);
void freeQueue(BinaryQueue* queue);

BinaryNode* createNode(int value);
BinaryNode* dfs_preorder(BinaryNode* root, int target);

char numOrPad(char c);
void serializeBinaryTree(char* filename, BinaryNode* root);
void getNodeVal(char* line, char* node, int num, int len);
BinaryNode* deserializeBinaryTree(char* filename);
void printBinaryTree(BinaryNode* root);

#endif
