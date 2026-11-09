#include "array.h"

// Создание пустого массива 
void Init(Array* a) {
    a->data = new string[MAX_SIZE];
    a->size = 0;
}

// Освобождение памяти 
void Free(Array* a) {
    delete[] a->data;
    a->data = nullptr;
    a->size = 0;
}

// Добавить в конец 
void pushBack(Array* a, string value) {
    if (a->size < MAX_SIZE) {
        a->data[a->size] = value;
        a->size++;
    }
}

// Добавить по индексу 
void pushAt(Array* a, int index, string value) {
    if (index < 0 || index > a->size) return;
    if (a->size >= MAX_SIZE) return;
    for (int i = a->size; i > index; i--) {
        a->data[i] = a->data[i - 1];
    }
    a->data[index] = value;
    a->size++;
}

// Получить по индексу 
string get(Array* a, int index) {
    if (index < 0 || index >= a->size) return "";
    return a->data[index];
}

// Заменить по индексу 
void set(Array* a, int index, string value) {
    if (index >= 0 && index < a->size) {
        a->data[index] = value;
    }
}

// Удалить по индексу 
void del(Array* a, int index) {
    if (index < 0 || index >= a->size) return;
    for (int i = index; i < a->size - 1; i++) {
        a->data[i] = a->data[i + 1];
    }
    a->size--;
}

// Длина 
int length(Array* a) {
    return a->size;
}

// Чтение 
void print(Array* a) {
    for (int i = 0; i < a->size; i++) {
        cout << a->data[i] << " ";
    }
    cout << endl;
}