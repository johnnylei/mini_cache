#include <stdlib.h>
#include "Event.h"
#include "HashTable.h"
#include "Link.h"

void * on(Event * event, const char * str, void * (* handler)(void *)) {
	hash_append(event->event_list, str, handler);
}

void * off(Event * event, const char *str) {
	hash_remove(event->event_list, str);
}

void * trigger(Event * event, const char * str, void * params) {
	Link * handlers;
	int ret = hash_lookup(event->event_list, str, (void **)&handlers);
	if (ret == FAILED) {
		return NULL;
	}

	void * (* handler)(void *);
	LinkNode * current = handlers->head;
	while(current) {
		handler = (void * (*)(void *))current->data;
		handler(params);
		current = current->next;
	}
}

Event * initEvent() {
	Event * event = (Event *)malloc(sizeof(Event));
	event->on = on;
	event->off = off;
	event->trigger = trigger;
	event->event_list = (HashTable *)malloc(sizeof(HashTable));
	hash_init(event->event_list);
	return event;
}
