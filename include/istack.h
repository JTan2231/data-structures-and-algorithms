#ifndef ISTACK
#define ISTACK

typedef struct illnode illnode;
typedef struct istack istack;

istack* create_istack();
void ispush(istack* s, int val);
int istop(istack* s);
void ispop(istack* s);

#endif
