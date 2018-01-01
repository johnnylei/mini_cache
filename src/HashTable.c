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

	LOG_MSG("[resize]\t org size:%i\tnew size:%i\n", org_size, ht->size);

	Bucket ** bucket = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	Bucket ** org_buckets = ht->buckets;
	ht->buckets = bucket;

	for (int i = 0; i < org_size; i++) {
		Bucket * cur = org_buckets[i];
		Bucket * tmp;

		while (cur) {
			hash_insert(ht, cur->key, cur->value);
		
			// check has next, and free space
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}

	free(org_buckets);
	LOG_MSG("[resize] done\n");
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

int hash_init(HashTable *ht) {
	ht->size = HASH_TABLE_INIT_SIZE;	
	ht->element_num = 0;
	ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	if(ht->buckets == NULL) return FAILED; 

	LOG_MSG("[init]\tsize:%i\n", ht->size);
	return SUCCESS;
}

int hash_init_with_size(HashTable *ht, int size) {
	ht->size = size;	
	ht->element_num = 0;
	ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	if(ht->buckets == NULL) return FAILED; 

	LOG_MSG("[init]\tsize:%i\n", ht->size);
	return SUCCESS;
}

int hash_append(HashTable *ht, const char *key, void *value) {
	// check if we need to resize the hashtable
	resize_hash_table_if_needed(ht);

	int index = HASH_INDEX(ht, key);

	Bucket *org_bucket = ht->buckets[index];
	Bucket *tmp_bucket = org_bucket;
	Bucket *pre = org_bucket;

	// check if the key exits already
	// if exits then update
	while (tmp_bucket) {
		if (strcmp(key, tmp_bucket->key) == 0) {
			Link * link = (Link *)tmp_bucket->value;
			LinkNode * linkNode = initLinkNode(value);
			link->append(link, linkNode);
			return SUCCESS;
		}

		pre = tmp_bucket;
		tmp_bucket = tmp_bucket->next;
	}

	Bucket * bucket = (Bucket *)malloc(sizeof(Bucket));

	Link *link = initLink();
	LinkNode * linkNode = initLinkNode(value);
	link->append(link, linkNode);
	bucket->key = (char *)malloc(sizeof(char));
	strcpy(bucket->key, key);
	bucket->value = (void *)link;
	bucket->next = NULL;

	ht->element_num += 1;

	// check collsion 
	if (org_bucket != NULL) {
		LOG_MSG("[collision]\tindex:%d key:%s\n", index, key);
		pre->next = bucket;
		return SUCCESS;
	}

	ht->buckets[index] = bucket;
	LOG_MSG("[insert]\tindex:%d key:%s\tht(num:%d)\n", index, key, ht->element_num);
	return SUCCESS;
}

int hash_insert(HashTable *ht, const char *key, void *value) {
	// check if we need to resize the hashtable
	resize_hash_table_if_needed(ht);

	int index = HASH_INDEX(ht, key);

	Bucket *org_bucket = ht->buckets[index];
	Bucket *tmp_bucket = org_bucket;
	Bucket *pre = org_bucket;

	// check if the key exits already
	// if exits then update
	while (tmp_bucket) {
		if (strcmp(key, tmp_bucket->key) == 0) {
			LOG_MSG("[update]\tindex%d key:%s\n", index, key);
			tmp_bucket->value = value;

			return SUCCESS;
		}

		pre = tmp_bucket;
		tmp_bucket = tmp_bucket->next;
	}

	Bucket * bucket = (Bucket *)malloc(sizeof(Bucket));

	bucket->key = (char *)malloc(sizeof(char));
	strcpy(bucket->key, key);
	bucket->value = value;
	bucket->next = NULL;

	ht->element_num += 1;

	// check collsion 
	if (org_bucket != NULL) {
		LOG_MSG("[collision]\tindex:%d key:%s\n", index, key);
		pre->next = bucket;
		return SUCCESS;
	}

	ht->buckets[index] = bucket;
	LOG_MSG("[insert]\tindex:%d key:%s\tht(num:%d)\n", index, key, ht->element_num);
	return SUCCESS;
}

int hash_lookup(HashTable *ht, const char *key, void ** result) {
	int index = HASH_INDEX(ht, key);
	Bucket * bucket = ht->buckets[index];

	if (NULL == bucket) {
		LOG_MSG("[lookup]\t key:%s\tfailed\t\n", key);
		return FAILED;
	}

	while (bucket) {
		if (strcmp(bucket->key, key) == 0) {
			*result = bucket->value;
			LOG_MSG("[lookup]\tfound %s\t index:%i value:%p\n", key, index, bucket->value);
		return SUCCESS;
		}

		bucket = bucket->next;
	}

	LOG_MSG("[lookup]\t key:%s\tfailed\t\n", key);
	return FAILED;
}

int hash_remove(HashTable * ht, const char * key) {
	int index = HASH_INDEX(ht, key);
	Bucket * bucket = ht->buckets[index];
	if (NULL == bucket) {
		LOG_MSG("{remove}\tcould not found element with key:%s\n", key);
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
			free(bucket);
			ht->element_num--;
			LOG_MSG("[remove]\t succeess key:%s\n", key);
			return SUCCESS;
		}

		pre = bucket;
		bucket = bucket->next;
	}

	LOG_MSG("[remove]\tcould not found element with key:%s\n", key);
	return FAILED;
}

int hash_destroy(HashTable *ht) {
	Bucket ** buckets = ht->buckets;
	Bucket * tmp, * bucket;
	for (int i = 0; i < ht->size; i++) {
		bucket = buckets[i];
		while (bucket) {
			tmp = bucket;
			bucket = bucket->next;
			free(tmp);
		}
	}
	
	free(ht->buckets);
	free(ht);
	return SUCCESS;
}
