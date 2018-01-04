#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *str = (char *)malloc(128);
	strcpy(str, "hello johnny");
	void * data = str;
	char * tmp = (char *)data;
	printf("%s\n", tmp);
}
