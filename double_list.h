#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <iostream>
#include <string>
using namespace std;

// Узел двусвязного списка
struct DNode {
    string data;
    DNode* prev;
    DNode* next;
};

// Обёртка списка
struct DList {
    DNode* head;
    DNode* tail;
};

void Init(DList* d);
void pushFront(DList* d, string value);
void pushBack(DList* d, string value);
void insertAfter(DList* d, DNode* cur, string value);
void insertBefore(DList* d, DNode* cur, string value);
void delHead(DList* d);
void delTail(DList* d);
void delAfter(DList* d, DNode* cur);
void delBefore(DList* d, DNode* cur);
void delByValue(DList* d, string value);
DNode* find(DList* d, string value);
DNode* findIndex(DList* d, int index);
string getAt(DList* d, int index);
void PrintForward(DList* d);
void PrintBackward(DList* d);
void PrintRecursive(DNode* cur);

#endif