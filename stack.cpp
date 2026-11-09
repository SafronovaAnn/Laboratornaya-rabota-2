#include "stack.h"

// Создание пустого стека 
void Init(Stack* s) {
    s->head = nullptr;
}

// Проверка на пустоту 
bool isEmpty(Stack* s) {
    return s->head == nullptr;
}

// Добавить на вершину (push) 
void push(Stack* s, string value) {
    SNode* newNode = new SNode{value, nullptr};
    newNode->next = s->head;
    s->head = newNode;
}

// Удалить с вершины (pop) 
SNode* pop(Stack* s) {
    if (isEmpty(s)) return nullptr;
    SNode* old = s->head;
    s->head = s->head->next;
    return old;
}

// Чтение 
void print(Stack* s) {
    SNode* cur = s->head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}
