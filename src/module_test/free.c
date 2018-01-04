#include <stdlib.h>

int main () {
	int *p = (int *)malloc(sizeof(int));
	int * z = p;
	free(z);
}
