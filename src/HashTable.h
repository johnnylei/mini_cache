#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#include "Link.h"
#include "common.h"

#define HASH_TABLE_INIT_SIZE 10
#define HASH_INDEX(ht, key) (hash_str((key)) % (ht)->size)
// #define DEBUG 1
#if defined(DEBUG)
#	define LOG_MSG printf
#else
# 	define LOG_MSG(...)
#endif

typedef struct _Bucket {
    char * key;
    void * value;
    struct _Bucket * next;
} Bucket;

typedef struct _HashTable {
    int size;
    int element_num;
    Bucket ** buckets;
} HashTable;

int hash_init(HashTable *ht);
int hash_init_with_size(HashTable *ht, int size);
int hash_lookup(HashTable *ht,const char *key, void **result);
int hash_insert(HashTable *ht,const char *key, void *value);
int hash_append(HashTable *ht,const char *key, void *value);
int hash_remove(HashTable *ht,const char *key);
int hash_destroy(HashTable *ht);

#endif
