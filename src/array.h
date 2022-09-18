#pragma once
#include "dvd.h"

typedef struct {
    Dvd* array;
    int len;
    int used;
} DvdArray;

void initDvdArray(DvdArray* a, int initialSize);
int insertDvdArray(DvdArray* a, Dvd element);
void delDvdArray(DvdArray* a);

typedef struct{
    int* array;
    int len;
    int used;
} IntArray;

void initIntArray(IntArray* a, int initialSize);
int insertIntArray(IntArray* a, int element);
void delIntArray(IntArray* a);
