#include <stdio.h>
#include <stdlib.h>
#include "../include/linear/vector.h"

int main(int argc, char** argv) {

	vector* v = vec_create(1, sizeof(int));

	int a = 10;
	vec_clear(v);

	int* res = (int*)vec_get_at(v, 0);

	printf("%zu", *res);


	vec_delete(&v);
	return 0;
}