#ifndef TASK6_1_HASH_H
#define TASK6_1_HASH_H

#include <iostream>
#include <string>
#include "array.h"
#include "double_list.h"

using namespace std;

struct ChainingHashTable {
    static const int TABLE_SIZE = 97;
    DList buckets[TABLE_SIZE];
    int hashFunc(const string& key);

    ChainingHashTable();
    void insert(const string& key, const string& value);
    string find(const string& key);
    void remove(const string& key);
    void print();

    // Геттер для сохранения
    DList* getBuckets() { return buckets; }
};

struct CuckooHashTable {
    static const int CAPACITY = 97;
    Array table1;
    Array table2;

    CuckooHashTable();
    ~CuckooHashTable();
    bool insert(const string& key, const string& value);
    string find(const string& key); 
    void print();
    void remove(const string& key);

    // Геттеры и хеш-функции для сохранения 
    Array* getTable1() { return &table1; }
    Array* getTable2() { return &table2; }
    int hash1(const string& key);
    int hash2(const string& key);
};

// Глобальные таблицы
extern ChainingHashTable globalChain;
extern CuckooHashTable globalCuckoo;

// Сохранение / загрузка
void saveHashes(const string& filename);
void loadHashes(const string& filename);

#endif