#ifndef TASK6_2_REHASH_H
#define TASK6_2_REHASH_H

#include <iostream>
#include <string>
#include <vector>
#include "double_list.h"

using namespace std;

struct RehashTable {
    vector<DList> table;
    int capacity;
    int size;
    const double LOAD_FACTOR = 0.9;
    
    int hashFunc(const string& key, int cap);
    void resize();

    RehashTable(int initial_capacity = 10);
    ~RehashTable();
    
    void insert(const string& key, const string& value);         
    void insertNoResize(const string& key, const string& value); 
    string find(const string& key);
    void remove(const string& key);
    void print();
    void setCapacity(int newCap);
    int getSize() { return size; }
    int getCapacity() { return capacity; }
    vector<DList>* getTable() { return &table; }
};

extern RehashTable globalRehash;

void saveRehash(const string& filename);
void loadRehash(const string& filename);

#endif