#ifndef __EVENT_H__
#define __EVENT_H__
#include "HashTable.h"
#include "Link.h"

typedef struct _event {
	HashTable * event_list;
	void * (* trigger)(struct _event *, const char *, void *);
	void * (* off)(struct _event *, const char *);
	void * (* on)(struct _event *, const char *, void * (* handler)(void *));
	void (* destroy)(void *);
} Event;

Event * initEvent();
#endif
