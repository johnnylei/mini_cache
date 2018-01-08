#ifndef __LINK_H__
#define __LINK_H__
typedef struct _linkNode {
	void * value;
	unsigned long valueSize;
	int valueType;
	struct _linkNode * next;
	void (* destroy)(void *);
	void (* destroyValue)(void *);
} LinkNode;

typedef struct _link {
	LinkNode * head;
	int size;
	int (* append)(struct _link *, LinkNode *);
	int (* del)(struct _link *, int);
	void (* destroy)(void *);
	void ** (* traversal)(struct _link *);
} Link;

Link * initLink();
LinkNode * initLinkNode(void * value, unsigned long valueSize, int valueType, void (* destroyValue)(void *));
#endif
