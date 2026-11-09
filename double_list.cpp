#include "double_list.h"

// Создание пустого списка 
void Init(DList* d) {
    d->head = nullptr;
    d->tail = nullptr;
}

// ДОБАВЛЕНИЕ (4 способа)

// В голову 
void pushFront(DList* d, string value) {
    DNode* newNode = new DNode{value, nullptr, d->head};
    if (d->head == nullptr) {
        d->head = newNode;
        d->tail = newNode;
        return;
    }
    d->head->prev = newNode;
    d->head = newNode;
}

// В хвост 
void pushBack(DList* d, string value) {
    DNode* newNode = new DNode{value, d->tail, nullptr};
    if (d->head == nullptr) {
        d->head = newNode;
        d->tail = newNode;
        return;
    }
    d->tail->next = newNode;
    d->tail = newNode;
}

// После указанного узла 
void insertAfter(DList* d, DNode* cur, string value) {
    if (cur == nullptr) return;

    DNode* newNode = new DNode{value, cur, cur->next};

    if (cur->next != nullptr) {
        cur->next->prev = newNode;
    } else {
        d->tail = newNode;
    }
    cur->next = newNode;
}

// До указанного узла 
void insertBefore(DList* d, DNode* cur, string value) {
    if (cur == nullptr) return;

    DNode* newNode = new DNode{value, cur->prev, cur};

    if (cur->prev != nullptr) {
        cur->prev->next = newNode;
    } else {
        d->head = newNode;
    }
    cur->prev = newNode;
}

// УДАЛЕНИЕ (5 способов)

// Голова 
void delHead(DList* d) {
    if (d->head == nullptr) return;

    DNode* old = d->head;
    d->head = d->head->next;

    if (d->head != nullptr) {
        d->head->prev = nullptr;
    } else {
        d->tail = nullptr;
    }
    delete old;
}

// Хвост 
void delTail(DList* d) {
    if (d->tail == nullptr) return;

    DNode* old = d->tail;
    d->tail = d->tail->prev;

    if (d->tail != nullptr) {
        d->tail->next = nullptr;
    } else {
        d->head = nullptr;
    }
    delete old;
}

// После указанного узла 
void delAfter(DList* d, DNode* cur) {
    if (cur == nullptr || cur->next == nullptr) return;

    DNode* old = cur->next;
    cur->next = old->next;

    if (old->next != nullptr) {
        old->next->prev = cur;
    } else {
        d->tail = cur;
    }
    delete old;
}

// До указанного узла 
void delBefore(DList* d, DNode* cur) {
    if (cur == nullptr || cur->prev == nullptr) return;

    DNode* old = cur->prev;
    cur->prev = old->prev;

    if (old->prev != nullptr) {
        old->prev->next = cur;
    } else {
        d->head = cur;
    }
    delete old;
}

// По значению 
void delByValue(DList* d, string value) {
    DNode* cur = d->head;
    while (cur != nullptr) {
        if (cur->data == value) {
            if (cur->prev != nullptr) cur->prev->next = cur->next;
            else d->head = cur->next;

            if (cur->next != nullptr) cur->next->prev = cur->prev;
            else d->tail = cur->prev;

            delete cur;
            return;
        }
        cur = cur->next;
    }
}

// ПОИСК 

DNode* find(DList* d, string value) {
    DNode* cur = d->head;
    while (cur != nullptr) {
        if (cur->data == value) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// Узел по индексу 
DNode* findIndex(DList* d, int index) {
    DNode* cur = d->head;
    int i = 0;
    while (cur != nullptr) {
        if (i == index) return cur;
        i++;
        cur = cur->next;
    }
    return nullptr;
}

// Значение по индексу 
string getAt(DList* d, int index) {
    DNode* n = findIndex(d, index);
    return n ? n->data : "";
}

// ЧТЕНИЕ (3 способа)

// 1. Вперёд 
void PrintForward(DList* d) {
    DNode* cur = d->head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

// 2. Назад 
void PrintBackward(DList* d) {
    DNode* cur = d->tail;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->prev;
    }
    cout << endl;
}

// 3. Рекурсивно 
void PrintRecursive(DNode* cur) {
    if (cur == nullptr) return;
    cout << cur->data << " ";
    PrintRecursive(cur->next);
}
