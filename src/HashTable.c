#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"
#include "common.h"
#include "Link.h"

static int hash_resize(HashTable *ht) {
	// double the size
	int org_size = ht->size;
	ht->size = org_size * 2;
	ht->element_num = 0;

	Bucket ** buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	Bucket ** org_buckets = ht->buckets;
	ht->buckets = buckets;

	for (int i = 0; i < org_size; i++) {
		Bucket * current = org_buckets[i];

		while (current) {
			ht->insert(ht, current);
			current = current->next;
		}
	}

	free(org_buckets);
	return SUCCESS;
}

static void resize_hash_table_if_needed(HashTable *ht) {
	if (ht->size - ht->element_num < 1) {
		hash_resize(ht);
	}
}

static int hash_str(const char *key) {
	int hash = 0;
	const char *cur = key;

	while (*cur != '\0') {
		hash += *cur;
		++cur;
	}

	return hash;
}

int hashInsert(HashTable *ht, Bucket * bucket) {
	// check if we need to resize the hashtable
	resize_hash_table_if_needed(ht);

	int index = HASH_INDEX(ht, bucket->key);

	Bucket *org_bucket = ht->buckets[index];
	Bucket *tmp_bucket = org_bucket;
	Bucket *pre = NULL;

	// check if the key exits already
	// if exits then update
	while (tmp_bucket) {
		if (strcmp(bucket->key, tmp_bucket->key) == 0) {
			if (pre == NULL) {
				bucket->next = tmp_bucket->next;
				ht->buckets[index] = bucket;
			} else {
				pre->next = bucket;
				bucket->next = tmp_bucket->next;
			}
			tmp_bucket->destroy(tmp_bucket);
			return SUCCESS;
		}

		pre = tmp_bucket;
		tmp_bucket = tmp_bucket->next;
	}

	ht->element_num += 1;

	// check collsion 
	if (org_bucket != NULL) {
		pre->next = bucket;
		return SUCCESS;
	}

	ht->buckets[index] = bucket;
	return SUCCESS;
}

int hashLookup(HashTable *ht, const char *key, void ** result) {
	int index = HASH_INDEX(ht, key);
	Bucket * bucket = ht->buckets[index];

	if (NULL == bucket) {
		return FAILED;
	}

	while (bucket) {
		if (strcmp(bucket->key, key) == 0) {
			*result = bucket->value;
			return SUCCESS;
		}

		bucket = bucket->next;
	}

	return FAILED;
}

int hashRemove(HashTable * ht, const char * key) {
	int index = HASH_INDEX(ht, key);
	Bucket * bucket = ht->buckets[index];
	if (NULL == bucket) {
		return FAILED;
	}

	Bucket * pre = NULL;
	while (bucket) {
		if(strcmp(key, bucket->key) == 0) {
			if (NULL == pre) {
				ht->buckets[index] = bucket->next;
			} else {
				pre->next = bucket->next;
			}
			bucket->destroy(bucket);
			ht->element_num--;
			return SUCCESS;
		}

		pre = bucket;
		bucket = bucket->next;
	}

	return FAILED;
}

void hashDestroy(void * object) {
	HashTable *ht = (HashTable *)object;
	Bucket * tmp, * bucket;
	for (int i = 0; i < ht->size; i++) {
		bucket = ht->buckets[i];
		if (NULL == bucket) {
			free(bucket);
			continue;
		}

		while (bucket) {
			tmp = bucket;
			bucket = bucket->next;
			tmp->destroy(tmp);
		}
	}
	
	free(ht->buckets);
	free(ht);
}

HashTable * initHash() {
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	ht->size = HASH_TABLE_INIT_SIZE;	
	ht->element_num = 0;
	ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	if(ht->buckets == NULL) return NULL; 

	ht->destroy = hashDestroy;
	ht->insert = hashInsert;
	ht->lookup= hashLookup;
	ht->remove = hashRemove;
	
	return ht;
}

HashTable * initHashWithSize(int size) {
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable)); 
	ht->size = size;	
	ht->element_num = 0;
	ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	if(ht->buckets == NULL) return NULL; 

	ht->destroy = hashDestroy;
	ht->insert = hashInsert;
	ht->lookup= hashLookup;
	ht->remove = hashRemove;

	return ht;
}

void bucketDestroy(void * object) {
	Bucket * bucket = (Bucket *)object;
	if (NULL != bucket->destroyValue) {
		bucket->destroyValue(bucket->value);
	}
	
	free(bucket);
}

Bucket * initBucket(const char * key, void * value, void (* destroyValue)(void *)) {
	Bucket * bucket = (Bucket *)malloc(sizeof(Bucket));
	bucket->key = (char *)malloc(sizeof(char));
	strcpy(bucket->key, key);
	bucket->value = value;
	bucket->valueSize = sizeof(value);
	bucket->destroy = bucketDestroy;
	bucket->destroyValue = destroyValue;
	return bucket;
}

