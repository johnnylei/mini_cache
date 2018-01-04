#include <stdlib.h> 
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

LinkNode * initLinkNode(void * value, void (* destroyValue)(void *)) {
	LinkNode * node = (LinkNode *)malloc(sizeof(LinkNode));
	node->value = value;
	node->valueSize = sizeof(value);
	node->next = NULL;
	node->destroy = linkNodeDestroy;
	node->destroyValue = destroyValue;
	return node;
}
