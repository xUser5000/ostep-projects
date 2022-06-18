#ifndef __pair_h__
#define __pair_h__

typedef struct __pair_t {
    void* first;
    void* second;
} pair_t;

pair_t* make_pair (void *first, void *second);

#endif
