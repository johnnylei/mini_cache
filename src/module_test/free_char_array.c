#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
	char * str = (char *)malloc(128 * sizeof(char));
	strcpy(str, "sssssssssssssssssssssssssssszzz");
	free(str);
	free(NULL);
	printf("sizeof str=%lu\n", sizeof(str));
	printf("strlen str=%lu\n", strlen(str));
	printf("sizeof int=%lu\n", sizeof(int));
	printf("sizeof char=%lu\n", sizeof(char));
	printf("sizeof long=%lu\n", sizeof(long));
}
