#include <stdlib.h> 
#include <string.h> 
#include <strings.h> 
#include "Link.h"
#include "common.h"

int linkAppend(Link * link, LinkNode * node) {
	link->size++;
	if (link->head == NULL) {
		link->head = node;
		return 1;
	}

	LinkNode * current = link->head;
	LinkNode * pre;
	while (current) {
		pre = current;
		current = current->next;
	}

	pre->next = node;
	return 1;
}

int linkDel(Link * link, int index) {
	if (index >= link->size) {
		return FAILED;
	}

	int i = 0;
	LinkNode * current = link->head;
	LinkNode * pre = NULL;
	while (current) {
		if (i <  index) {
			pre = current;
			current = current->next;
			i++;
			continue;
		}

		if (pre == NULL) {
			link->head = current->next;
			current->destroy(current);
			break;
		}

		pre->next = current->next;
		current->destroy(current);
		break;
	}

	link->size--;
	return SUCCESS;
}

void linkDestroy(void * object) {
	Link * link = (Link *)object;
	if(link->size == 0) {
		return ;
	}

	LinkNode * current = link->head;
	while (current) {
		link->head = current->next;
		current->destroy(current);
		current = link->head;
		link->size--;
	}

	free(link);
}

Link * initLink() {
	Link * link = (Link *)malloc(sizeof(Link));
	bzero(link, sizeof(Link));
	link->head = NULL;
	link->append = linkAppend;
	link->del = linkDel;
	link->destroy = linkDestroy;
	link->size = 0;
	return link;
}

void linkNodeDestroy(void * object) {
	LinkNode * node = (LinkNode *)object;
	if (NULL != node->destroyValue) {
		node->destroyValue(node->value);
	}

	free(node);
}

LinkNode * initLinkNode(void * value, unsigned long valueSize, int valueType, void (* destroyValue)(void *)) {
	LinkNode * node = (LinkNode *)malloc(sizeof(LinkNode));
	bzero(node, sizeof(LinkNode));

	node->valueSize = valueSize;
	node->valueType = valueType;
	if (valueSize == 0) {
		node->value = value;
	} else {
		node->value = malloc(valueSize);
		memset(node->value, '\0', valueSize);
		memcpy(node->value, value, valueSize);
	}

	node->next = NULL;
	node->destroyValue = destroyValue;
	node->destroy = linkNodeDestroy;
	return node;
}
