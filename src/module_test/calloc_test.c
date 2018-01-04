#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
	char ** str = (char **)calloc(6, sizeof(char *));
	
	// char * test = (char *)malloc(128 * sizeof(char));
	char * test = (char *)malloc(sizeof(char));
	strcpy(test, "ssssssssssssssssssssssxxxxx");
	free(str);


	str = (char **)calloc(7, sizeof(char *));

	free(str);


	str = (char **)calloc(8, sizeof(char *));

	free(str);

	str = (char **)calloc(9, sizeof(char *));

	free(str);

	str = (char **)calloc(6, sizeof(char *));

	free(str);
	return 0;
}
