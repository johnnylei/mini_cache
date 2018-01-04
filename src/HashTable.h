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
	unsigned long valueSize;
	void (* destroy)(void *);
	void (* destroyValue)(void *);
    struct _Bucket * next;
} Bucket;

typedef struct _HashTable {
    int size;
    int element_num;
    Bucket ** buckets;
	int (* insert)(struct _HashTable *, Bucket *);
	int (* remove)(struct _HashTable *, const char *);
	void (* destroy)(void *);
	int (* lookup)(struct _HashTable *, const char *, void **);
} HashTable;

Bucket * initBucket(const char *key, void *value, void (* destroyValue)(void *));
HashTable * initHash();
HashTable * initHashWithSize(int size);

#endif
