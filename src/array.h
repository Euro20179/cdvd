#pragma once
#include "dvd.h"

typedef struct {
    Dvd* array;
    int len;
    int used;
} Array;

void initArray(Array* a, int initialSize);
int insertArray(Array* a, Dvd element);
void delArray(Array* a);
