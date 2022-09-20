#include <stdio.h>
#include <stdlib.h>

#include "array.h"

#include "dvd.h"

void initDvdArray(DvdArray *a, int initialSize) {
  a->array = malloc(initialSize * sizeof(Dvd));
  a->used = 0;
  a->len = initialSize;
}

int insertDvdArray(DvdArray *a, Dvd element) {
  if (a->used == a->len) {
      if(a->len == 0){
          a->len = 1;
      }
      else{
        a->len *= 2;
      }
    void *mem = realloc(a->array, a->len * sizeof(Dvd));
    if (!mem) {
      free(mem);
      return 1;
    }
    a->array = mem;
  }
  a->array[a->used++] = element;
  return 0;
}

void popDvdArrayItem(DvdArray* a){
    a->used--;
    if(a->used < a->len){
        a->len = a->used;
    }
    void* mem = realloc(a->array, a->len * sizeof(Dvd));
    if(!mem){
        free(mem);
    }
    else{
        a->array = mem;
    }
}

void delDvdArray(DvdArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->len = 0;
}


void initIntArray(IntArray *a, int initialSize) {
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->len = initialSize;
}

int insertIntArray(IntArray* a, int element) {
  if (a->used == a->len) {
      if(a->len == 0){
          a->len = 1;
      }
      else{
        a->len *= 2;
      }
    void *mem = realloc(a->array, a->len * sizeof(int));
    if (!mem) {
      free(mem);
      return 1;
    }
    a->array = mem;
  }
  a->array[a->used++] = element;
  return 0;
}

void delIntArray(IntArray* a) {
  free(a->array);
  a->array = NULL;
  a->used = a->len = 0;
}
