#ifndef __LINK_H__
#define __LINK_H__
typedef struct _linkNode {
	void * data;
	struct _linkNode * next;
} LinkNode;

typedef struct _link {
	LinkNode * head;
	int (* append)(struct _link *, LinkNode *);
	void (* traversal)(struct _link *);
} Link;

Link * initLink();
LinkNode * initLinkNode(void * data);
#endif
