#include "task6_2_rehash.h"
#include <fstream>
#include <sstream>

RehashTable::RehashTable(int initial_capacity) {
    capacity = initial_capacity;
    size = 0;
    table.resize(capacity);
    for (int i = 0; i < capacity; i++) {
        Init(&table[i]);
    }
}

RehashTable::~RehashTable() {
    for (int i = 0; i < capacity; i++) {
        while (table[i].head != nullptr) {
            DNode* n = table[i].head;
            table[i].head = table[i].head->next;
            delete n;
        }
    }
}

int RehashTable::hashFunc(const string& key, int cap) {
    unsigned long hash = 0;
    for (char c : key) hash = hash * 31 + c;
    return hash % cap;
}


//Основная функция рекрутизации
void RehashTable::resize() {
    int new_capacity = capacity * 2;
    cout << "Реструктуризация: " << capacity << " -> " << new_capacity << endl;

    vector<DList> new_table(new_capacity);
    for (int i = 0; i < new_capacity; i++) {
        Init(&new_table[i]);
    }

    for (int i = 0; i < capacity; i++) {
        DNode* cur = table[i].head;
        while (cur != nullptr) {
            string record = cur->data;
            int sep = record.find('\x01'); 
            string key = record.substr(0, sep);
            int new_idx = hashFunc(key, new_capacity);
            pushBack(&new_table[new_idx], record); 
            cur = cur->next;
        }
    }
    for (int i = 0; i < capacity; i++) {
        while (table[i].head != nullptr) {
            DNode* n = table[i].head;
            table[i].head = table[i].head->next;
            delete n;
        }
    }

    table = move(new_table);
    capacity = new_capacity;
}

// Для загрузки из файла
void RehashTable::setCapacity(int newCap) {
    // Очищаем старую память
    for (int i = 0; i < capacity; i++) {
        while (table[i].head != nullptr) {
            DNode* n = table[i].head;
            table[i].head = table[i].head->next;
            delete n;
        }
    }
    
    // Меняем размер и создаем новые бакеты
    capacity = newCap;
    size = 0;
    table.clear();
    table.resize(capacity);
    for (int i = 0; i < capacity; i++) {
        Init(&table[i]);
    }
}


// Вставка (с проверкой на 90%)
void RehashTable::insert(const string& key, const string& value) {
    int idx = hashFunc(key, capacity);
    string record = key + '\x01' + value; 

    DNode* cur = table[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01'); 
        if (cur->data.substr(0, sep) == key) return;
        cur = cur->next;
    }
    pushBack(&table[idx], record);
    size++;

    if ((double)size / capacity >= LOAD_FACTOR) {
        resize();
    }
}

// Вставка при загрузке из файла 
void RehashTable::insertNoResize(const string& key, const string& value) {
   int idx = hashFunc(key, capacity);
    string record = key + '\x01' + value; 
    DNode* cur = table[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01');
        if (cur->data.substr(0, sep) == key) return;
        cur = cur->next;
    }
    pushBack(&table[idx], record);
    size++;
}

// Поиск значения по ключу
string RehashTable::find(const string& key) { 
    int idx = hashFunc(key, capacity);
    DNode* cur = table[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01');
        if (cur->data.substr(0, sep) == key) {
            return cur->data.substr(sep + 1); 
        }
        cur = cur->next;
    }
    return "";
}

void RehashTable::remove(const string& key) {
    int idx = hashFunc(key, capacity);
    DNode* cur = table[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01'); 
        if (cur->data.substr(0, sep) == key) {
            delByValue(&table[idx], cur->data); 
            size--;
            return;
        }
        cur = cur->next;
    }
}

void RehashTable::print() {
    cout << "Хеш-таблица (size: " << size << ", cap: " << capacity << "):" << endl;
    for (int i = 0; i < capacity; i++) {
        if (table[i].head != nullptr) {
            cout << "[" << i << "]: ";
            DNode* cur = table[i].head;
            while (cur) {
                string record = cur->data;
                int sep = record.find('\x01');
                if (sep != string::npos) record[sep] = ':'; 
                cout << record << " ";
                cur = cur->next;
            }
            cout << endl;
        }
    }
}

RehashTable globalRehash(10);

void saveRehash(const string& filename) {
    // Перезаписываем файл 
    ifstream in(filename);
    vector<string> lines;
    string line;
    while (getline(in, line)) {
        if (line.find("REHASH ") != 0) lines.push_back(line); // Оставляем всё, кроме старых REHASH
    }
    in.close();

    ofstream out(filename);
    for (const string& l : lines) out << l << "\n";

    // Записываем новую строку
    out << "REHASH " << globalRehash.getCapacity() << " ";
    vector<DList>* table = globalRehash.getTable();
    for (int i = 0; i < globalRehash.getCapacity(); i++) {
        DNode* cur = (*table)[i].head;
        while (cur) {
            string record = cur->data;
            int sep = record.find('\x01');
            if (sep != string::npos) record[sep] = ' '; 
            out << record << " "; 
            cur = cur->next;
        }
    }
    out << "\n";
}

void loadRehash(const string& filename) {
    ifstream f(filename);
    if (!f) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tag;
        int cap;
        ss >> tag >> cap;

        if (tag == "REHASH") {
            // Меняем размер таблицы на тот, что сохранен в файле
            globalRehash.setCapacity(cap);
            
            string key, val;
            while (ss >> key >> val) { 
                globalRehash.insertNoResize(key, val);  
            }
        }
    }
}