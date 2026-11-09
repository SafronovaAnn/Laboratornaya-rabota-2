#ifndef TASK2_SET_H
#define TASK2_SET_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "double_list.h"

using namespace std;

struct MySet {
    static const int TABLE_SIZE = 97;
    DList buckets[TABLE_SIZE];

    MySet();
    int hashFunc(const string& key);
    void add(const string& value);
    void remove(const string& value);
    bool contains(const string& value);
    void print();
};

extern unordered_map<string, MySet> globalSets;

void saveSetsToFile(const string& filename);
void loadSetsFromFile(const string& filename);

#endif