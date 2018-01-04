#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	char * s = (char *)malloc(100);
	void * p = malloc(sizeof(s));
	strcpy(s, "hello johnny");
	memcpy(p, s, 100);
	free(s);
	printf("%s\n", (char *)p);
}
