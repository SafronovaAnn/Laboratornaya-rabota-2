#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string>
using namespace std;

#define MAX_SIZE 1000

// Структура динамического массива
struct Array {
    string* data;
    int size;
};

void Init(Array* a);
void Free(Array* a);
void pushBack(Array* a, string value);
void pushAt(Array* a, int index, string value);
string get(Array* a, int index);
void set(Array* a, int index, string value);
void del(Array* a, int index);
int length(Array* a);
void print(Array* a);

#endif