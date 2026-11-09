#ifndef TASK7_LFU_H
#define TASK7_LFU_H

#include <iostream>
#include <string>
#include <vector>
#include "double_list.h"
#include "task6_1_hash.h"

using namespace std;

struct LFUNode {
    string data;       // "ключ:значение"
    int freq;          // частота
    DNode* listNode;   // указатель на узел главного списка
    bool isChain;
};

struct LFUCache {
    int capacity;
    DList list; 
    
    // Таблицы из 6.1
    ChainingHashTable chain; 
    CuckooHashTable cuckoo;

    LFUCache(int cap);
    ~LFUCache();
    
    int get(int key);
    void putInChain(int key, int value);
    void putInCuckoo(int key, int value);
    void printCache();
    void setCapacity(int newCap);
    
    LFUNode* findNode(const string& key);
    
    // Для сохранения/загрузки
    DList* getList() { return &list; }
    int getCapacity() { return capacity; }
    ChainingHashTable* getChain() { return &chain; }
    CuckooHashTable* getCuckoo() { return &cuckoo; }
    
    // Общая функция создания узла
    LFUNode* createNode(int key, int value); // было void
};

extern LFUCache globalLFU;
void saveLFU(const string& filename);
void loadLFU(const string& filename);

#endif