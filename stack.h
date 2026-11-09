#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
using namespace std;

// Узел стека
struct SNode {
    string data;
    SNode* next;
};

// Стек 
struct Stack {
    SNode* head;
};

void Init(Stack* s);
bool isEmpty(Stack* s);
void push(Stack* s, string value);
SNode* pop(Stack* s);
void print(Stack* s);

#endif