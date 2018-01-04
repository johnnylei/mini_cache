#include <stdio.h>
#include "../Event.h" 

void * print_str(void * str) {
	char * _str = (char *)str;
	printf("%s\n", _str);
}

void * print_str_1(void * str) {
	char * _str = (char *)str;
	printf("%s\n", _str);
}

int main() {
	Event * event = initEvent();
	event->on(event, "print_str", print_str);
	event->on(event, "print_str", print_str_1);
 	event->off(event, "print_str");
	char str[] = "hello fucker";
	event->trigger(event, "print_str", str);
}

