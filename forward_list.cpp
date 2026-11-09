#include "forward_list.h"

// Создание пустого списка 
void Init(Node** head) {
    *head = nullptr;
}

// Добавление

// В голову 
void pushFront(Node** head, string value) {
    Node* newNode = new Node{value, *head};
    *head = newNode;
}

// В хвост 
void pushBack(Node** head, string value) {
    Node* newNode = new Node{value, nullptr};
    if (*head == nullptr) {
        *head = newNode;
        return;
    }
    Node* cur = *head;
    while (cur->next != nullptr) {
        cur = cur->next;
    }
    cur->next = newNode;
}

// После указанного узла 
void insertAfter(Node* cur, string value) {
    if (cur == nullptr) return;
    Node* newNode = new Node{value, cur->next};
    cur->next = newNode;
}

// До указанного узла 
void insertBefore(Node** head, Node* cur, string value) {
    if (cur == nullptr || *head == nullptr) return;
    if (cur == *head) {
        pushFront(head, value);
        return;
    }
    Node* prev = *head;
    while (prev->next != cur) {
        prev = prev->next;
    }
    Node* newNode = new Node{value, cur};
    prev->next = newNode;
}

// Удаление

// Голова 
void delHead(Node** head) {
    if (*head == nullptr) return;
    Node* old = *head;
    *head = (*head)->next;
    delete old;
}

// Хвост 
void delTail(Node** head) {
    if (*head == nullptr) return;
    if ((*head)->next == nullptr) {
        delete *head;
        *head = nullptr;
        return;
    }
    Node* cur = *head;
    while (cur->next->next != nullptr) {
        cur = cur->next;
    }
    delete cur->next;
    cur->next = nullptr;
}

// После указанного узла 
void delAfter(Node* cur) {
    if (cur == nullptr || cur->next == nullptr) return;
    Node* old = cur->next;
    cur->next = old->next;
    delete old;
}

// До указанного узла 
void delBefore(Node** head, Node* cur) {
    if (cur == nullptr || cur == *head) return;

    if ((*head)->next == cur) {
        delHead(head);
        return;
    }

    Node* prev = *head;
    while (prev->next->next != cur) {
        prev = prev->next;
    }
    delAfter(prev);
}

// По значению 
void delByValue(Node** head, string value) {
    if (*head == nullptr) return;
    if ((*head)->data == value) {
        delHead(head);
        return;
    }
    Node* cur = *head;
    while (cur->next != nullptr) {
        if (cur->next->data == value) {
            Node* old = cur->next;
            cur->next = old->next;
            delete old;
            return;
        }
        cur = cur->next;
    }
}

// Поиск по значению
Node* find(Node* head, string value) {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->data == value) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// Получить узел по индексу 
Node* findIndex(Node* head, int index) {
    Node* cur = head;
    int i = 0;
    while (cur != nullptr) {
        if (i == index) return cur;
        i++;
        cur = cur->next;
    }
    return nullptr;
}

// Получить значение по индексу 
string getAt(Node* head, int index) {
    Node* n = findIndex(head, index);
    return n ? n->data : "";
}

// Чтение

// 1. Обычное 
void print(Node* head) {
    Node* cur = head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

// 2. Рекурсивное 
void printRecursive(Node* cur) {
    if (cur == nullptr) return;
    cout << cur->data << " ";
    printRecursive(cur->next);
}

// 3. В обратном порядке 
void printReverse(Node* cur) {
    if (cur == nullptr) return;
    printReverse(cur->next);
    cout << cur->data << " ";
}
