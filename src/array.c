#include <stdio.h>
#include <stdlib.h>

#include "array.h"

#include "dvd.h"

void initArray(Array* a, int initialSize){
    a->array = malloc(initialSize * sizeof(Dvd));
    a->used = 0;
    a->len = initialSize;
}

int insertArray(Array* a, Dvd element) {
    if(a->used == a->len){
	a->len *= 2;
	void* mem = realloc(a->array, a->len * sizeof(Dvd));
	if(!mem) { free(mem); return 1; }
	a->array = mem;
    }
    a->array[a->used++] = element;
    return 0;
}

void delArray(Array* a){
    free(a->array);
    a->array = NULL;
    a->used = a->len = 0;
}
