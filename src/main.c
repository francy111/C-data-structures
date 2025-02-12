#include <stdio.h>
#include <stdlib.h>

#include "../include/vector.h"

int main(int argc, char** argv) {

	Vector* v = createVector(3, sizeof(int));

	int tmp = 10;
	insertAt(v, &tmp, 0);
	tmp = 11;
	insertAt(v, &tmp, 1);
	tmp = 12;
	insertAt(v, &tmp, 2);

	for (int i = 0; i < getVectorSize(v); i++) {
		tmp = *(int*)getAt(v, i);

		printf("(%d)\n", tmp);
	}

	tmp = 4;
	printf("11 si trova alla posizione %d", contains(v, &tmp) - 1);
	printf("La posizione 1 e' vuota? %d\n", isIndexEmpty(v, 1));

	removeAt(v, 1);
	printf("Adesso l'ho svuotata %d\n", isIndexEmpty(v, 1));

	deleteVector(&v);

	return 0;
}