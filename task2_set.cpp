#include "task2_set.h"
#include <fstream>
#include <sstream>

//Реализация множества

MySet::MySet() {
    // Инициализация бакетов
    for (int i = 0; i < TABLE_SIZE; i++) {
        Init(&buckets[i]);
    }
}

int MySet::hashFunc(const string& key) {
    // Хеш-функция Горнера
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % TABLE_SIZE;
}

void MySet::add(const string& value) {
    int idx = hashFunc(value);
    if (find(&buckets[idx], value) == nullptr) {
        pushBack(&buckets[idx], value); 
    }
}

void MySet::remove(const string& value) {
    int idx = hashFunc(value);
    delByValue(&buckets[idx], value); 
}

bool MySet::contains(const string& value) {
    int idx = hashFunc(value);
    return find(&buckets[idx], value) != nullptr; 
}

void MySet::print() {
    cout << "{";
    bool first = true;
    for (int i = 0; i < TABLE_SIZE; i++) {
        DNode* cur = buckets[i].head;
        while (cur) {
            if (!first) cout << ", ";
            cout << cur->data;
            first = false;
            cur = cur->next;
        }
    }
    cout << "}" << endl;
}

// Глобальная база множеств
unordered_map<string, MySet> globalSets;


void saveSetsToFile(const string& filename) {
    ofstream f(filename);
    if (!f) return;

    for (const auto& pair : globalSets) {
        f << "SET " << pair.first << " ";
        for (int i = 0; i < MySet::TABLE_SIZE; i++) {
            DNode* cur = pair.second.buckets[i].head;
            while (cur) {
                f << cur->data << " ";
                cur = cur->next;
            }
        }
        f << "\n";
    }
}

void loadSetsFromFile(const string& filename) {
    ifstream f(filename);
    if (!f) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tag, name;
        ss >> tag >> name;

        if (tag == "SET") {
            string val;
            while (ss >> val) {
                globalSets[name].add(val); 
            }
        }
    }
}
