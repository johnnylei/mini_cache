#include <stdlib.h> 
#include "Link.h"

int append(Link * link, LinkNode * node) {
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

Link * initLink() {
	Link * link = (Link *)malloc(sizeof(Link));
	link->head = NULL;
	link->append = append;
	link->size = 0;
	return link;
}

LinkNode * initLinkNode(void * data) {
	LinkNode * node = (LinkNode *)malloc(sizeof(LinkNode));
	node->data = data;
	node->next = NULL;
	return node;
}

