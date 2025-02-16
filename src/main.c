#include <stdio.h>
#include <stdlib.h>

#include "../include/linear/vector.h"
#include "../include/linear/linkedlist.h"

void printint(void* val);
void* double_int(void* val);

int main(int argc, char** argv) {

	linkedlist* ll = ll_create(sizeof(int));

	int a = 0;
	for (int i = 0; i < 10; i++) {
		ll_insert_at(ll, &a, i);
		a++;
	}

	ll_for_each(ll, *printint);

	linkedlist* l2 = ll_map(ll, *double_int);
	ll_for_each(l2, *printint);

	ll_delete(&ll);
	ll_delete(&l2);

	return 0;
}

void printint(void* val) {

	int x = *(int*)val;
	printf("%d ", x);
}
void* double_int(void* val) {
	*(int*)val = (*(int*)val) * 2;
	return val;
}