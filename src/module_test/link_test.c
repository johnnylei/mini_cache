#include <stdio.h>
#include "../Link.h"

void printfIntLink(Link *link) {
    LinkNode * current = link->head;
    while(current) {
        int *data = (int *)current->data;
        printf("%d\n", *data);
		current = current->next;
    }   
}

int main() {
	int arr[] = {1, 2, 3, 4, 5, 100, 200};
	Link * link = initLink();
	link->traversal = printfIntLink;
	int len = sizeof(arr) / sizeof(int);
	for (int i = 0; i < len; i++) {
		LinkNode * node = initLinkNode((void *)&arr[i], NULL);
		link->append(link, node);
	}

	link->traversal(link);
	link->del(link, 1);
	link->del(link, 1);
	link->traversal(link);
	link->destroy(link);
	link->traversal(link);
}
