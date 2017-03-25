#include <stdlib.h>
#include <stdio.h>

int main(){
	int * data = malloc(100 * sizeof(int));
	printf("pointer_before: %p\n", data);
	data[0] = 500;
	free(data);
	printf("%p:%d\n", data, data[0]);
}