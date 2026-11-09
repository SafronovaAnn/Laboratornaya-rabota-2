#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>
#include <string>
using namespace std;

// Узел односвязного списка
struct Node {
    string data;
    Node* next;
};

void Init(Node** head);
void pushFront(Node** head, string value);
void pushBack(Node** head, string value);
void insertAfter(Node* cur, string value);
void insertBefore(Node** head, Node* cur, string value);
void delHead(Node** head);
void delTail(Node** head);
void delAfter(Node* cur);
void delBefore(Node** head, Node* cur);
void delByValue(Node** head, string value);
Node* find(Node* head, string value);
Node* findIndex(Node* head, int index);
string getAt(Node* head, int index);
void print(Node* head);
void printRecursive(Node* cur);
void printReverse(Node* cur);

#endif