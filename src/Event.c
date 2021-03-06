#include <stdlib.h>
#include <strings.h>
#include "Event.h"
#include "common.h"
#include "HashTable.h"
#include "Link.h"

void * on(Event * event, const char * str, void * (* handler)(void *)) {
	HashTable * eventList = event->event_list;
	
	Link * handlers;
	LinkNode * node = initLinkNode(handler, 0, DATA_TYPE_CALLBACK, NULL);
	Bucket * result;
	int ret = eventList->lookup(eventList, str, &result);
	if (ret != FAILED) {
		handlers = (Link *)result->value;
		handlers->append(handlers, node);
		return NULL;
	}

	handlers = initLink();
	handlers->append(handlers, node);
	Bucket * bucket = initBucket(str, handlers, sizeof(Link), DATA_TYPE_LINK, handlers->destroy);
	eventList->insert(eventList, bucket);
}

void * off(Event * event, const char *str) {
	event->event_list->remove(event->event_list, str);
}

void * trigger(Event * event, const char * str, void * params) {
	Bucket * result;
	int ret = event->event_list->lookup(event->event_list, str, &result);
	if (ret == FAILED) {
		return NULL;
	}

	Link * handlers = (Link *)result->value;
	void * (* handler)(void *);
	LinkNode * current = handlers->head;
	while(current) {
		handler = (void * (*)(void *))current->value;
		handler(params);
		current = current->next;
	}
}

void eventDestroy(void * object) {
	Event * event = (Event *)object;
	event->event_list->destroy(event->event_list);
	free(event);
}

Event * initEvent() {
	Event * event = (Event *)malloc(sizeof(Event));
	bzero(event, sizeof(Event));
	event->on = on;
	event->off = off;
	event->trigger = trigger;
	event->destroy = eventDestroy;
	event->event_list = initHash(event->event_list);
	return event;
}
