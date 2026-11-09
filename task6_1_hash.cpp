#include "task6_1_hash.h"
#include <fstream>
#include <sstream>
#include <vector>

// Реализация цепочек
ChainingHashTable::ChainingHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Init(&buckets[i]); 
    }
}

int ChainingHashTable::hashFunc(const string& key) {
    unsigned long hash = 0;
    for (char c : key) hash = hash * 31 + c;
    return hash % 3;
}

void ChainingHashTable::insert(const string& key, const string& value) {
    int idx = hashFunc(key);
    // Формируем строку "ключ\x01значение"
    string record = key + '\x01' + value;
    
    // Проверяем, есть ли уже такой ключ
    DNode* cur = buckets[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01');
        string storedKey = cur->data.substr(0, sep);
        if (storedKey == key) return; 
        cur = cur->next;
    }
    pushBack(&buckets[idx], record);
}
 
string ChainingHashTable::find(const string& key) {
    int idx = hashFunc(key);
    DNode* cur = buckets[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01');
        string storedKey = cur->data.substr(0, sep);
        if (storedKey == key) {
            return cur->data.substr(sep + 1); 
        }
        cur = cur->next;
    }
    return ""; 
}
 
void ChainingHashTable::remove(const string& key) {
    int idx = hashFunc(key);
    DNode* cur = buckets[idx].head;
    while (cur) {
        int sep = cur->data.find('\x01');
        string storedKey = cur->data.substr(0, sep);
        if (storedKey == key) {
            delByValue(&buckets[idx], cur->data);
            return;
        }
        cur = cur->next;
    }
}

void ChainingHashTable::print() {
    cout << "--- Chaining Hash Table ---" << endl;
    bool empty = true;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (buckets[i].head != nullptr) {
            cout << "[" << i << "]: ";
            DNode* cur = buckets[i].head;
            while (cur) {
                string record = cur->data;
                int sep = record.find('\x01');
                if (sep != string::npos) record[sep] = ':';
                cout << record << " ";
                cur = cur->next;
            }
            cout << endl;
            empty = false;
        }
    }
    if (empty) cout << "(пусто)" << endl;
}

// Реализация кукушки
CuckooHashTable::CuckooHashTable() {
    Init(&table1); 
    Init(&table2); 
    for (int i = 0; i < CAPACITY; i++) {
        pushBack(&table1, "");
        pushBack(&table2, "");
    }
}

CuckooHashTable::~CuckooHashTable() {
    Free(&table1); 
    Free(&table2); 
}

int CuckooHashTable::hash1(const string& key) {
    unsigned long hash = 0;
    for (char c : key) hash = hash * 31 + c;
    return hash % 3;
}

int CuckooHashTable::hash2(const string& key) {
    unsigned long hash = 0;
    for (char c : key) hash = hash * 53 + c;
    return hash % 3;
}

bool CuckooHashTable::insert(const string& key, const string& value) {
    if (find(key) != "") return true;
    string current = key+'\x01'+value;
    string currentKey = key; 

    int max_loops = 50;
    for (int i = 0; i < max_loops; i++) {
        int pos1 = hash1(currentKey);
        if (table1.data[pos1] == "") {
            set(&table1, pos1, current); 
            return true;
        }
        string evicted = table1.data[pos1];
        set(&table1, pos1, current);
        current = evicted;

        int sep1 = current.find('\x01');
        currentKey = current.substr(0, sep1);

        int pos2 = hash2(currentKey);
        if (table2.data[pos2] == "") {
            set(&table2, pos2, current); 
            return true;
        }
        evicted = table2.data[pos2];
        set(&table2, pos2, current);
        current = evicted;

        int sep2 = current.find('\x01');
        currentKey = current.substr(0, sep2);
    }
    cerr << "Ошибка: зацикливание при вставке " << key << endl;
    return false;
}

string CuckooHashTable::find(const string& key) {
    // Проверяем первую таблицу
    int pos1 = hash1(key);
    if (table1.data[pos1] != "") {
        int sep = table1.data[pos1].find('\x01');
        string storedKey = table1.data[pos1].substr(0, sep);
        if (storedKey == key) {
            return table1.data[pos1].substr(sep + 1); // Возвращаем значение
        }
    }

    // Проверяем вторую таблицу
    int pos2 = hash2(key);
    if (table2.data[pos2] != "") {
        int sep = table2.data[pos2].find('\x01');
        string storedKey = table2.data[pos2].substr(0, sep);
        if (storedKey == key) {
            return table2.data[pos2].substr(sep + 1); // Возвращаем значение
        }
    }

    return ""; 
}

void CuckooHashTable::remove(const string& key) {
    // Проверяем первую таблицу
    int pos1 = hash1(key);
    if (table1.data[pos1] != "") {
        int sep = table1.data[pos1].find('\x01');
        string storedKey = table1.data[pos1].substr(0, sep);
        if (storedKey == key) {
            set(&table1, pos1, ""); 
            return;
        }
    }

    // Проверяем вторую таблицу
    int pos2 = hash2(key);
    if (table2.data[pos2] != "") {
        int sep = table2.data[pos2].find('\x01');
        string storedKey = table2.data[pos2].substr(0, sep);
        if (storedKey == key) {
            set(&table2, pos2, ""); 
            return;
        }
    }
}

void CuckooHashTable::print() {
    cout << "--- Cuckoo Hash Table 1 ---" << endl;
    bool empty1 = true;
    for (int i = 0; i < CAPACITY; i++) {
        if (table1.data[i] != "") {
            string record = table1.data[i];
            int sep = record.find('\x01');
            if (sep != string::npos) record[sep] = ':';
            cout << "[" << i << "]: " << record << endl;
            empty1 = false;
        }
    }
    if (empty1) cout << "(пусто)" << endl;

    cout << "--- Cuckoo Hash Table 2 ---" << endl;
    bool empty2 = true;
    for (int i = 0; i < CAPACITY; i++) {
        if (table2.data[i] != "") {
            string record = table2.data[i];
            int sep = record.find('\x01');
            if (sep != string::npos) record[sep] = ':';
            cout << "[" << i << "]: " << record << endl;
            empty2 = false;
        }
    }
    if (empty2) cout << "(пусто)" << endl;
}
// Глобальные таблицы
ChainingHashTable globalChain;
CuckooHashTable globalCuckoo;

// Сохранение/загрузка
void saveHashes(const string& filename) {
    ifstream in(filename);
    vector<string> lines;
    string line;
    while (getline(in, line)) {
        if (line.find("CHAIN") != 0 && line.find("CUCKOO") != 0) {
            lines.push_back(line);
        }
    }
    in.close();
 
    // Перезаписываем файл
    ofstream out(filename);
    for (const string& l : lines) out << l << "\n";
 
    // Записываем новые CHAIN
    out << "CHAIN";
    for (int i = 0; i < 97; i++) {
        DNode* cur = globalChain.getBuckets()[i].head;
        while (cur) { 
            string record = cur->data;
            int sep = record.find('\x01');
            if (sep != string::npos) {
                record[sep] = ' '; 
            }
            out << " " << record; 
            cur = cur->next; 
        }
    }
    out << "\n";
 
    // Записываем новые CUCKOO
        out << "CUCKOO";
    // Сохраняем из первой таблицы
    for (int i = 0; i < 97; i++) {
        if (globalCuckoo.getTable1()->data[i] != "") {
            string record = globalCuckoo.getTable1()->data[i];
            int sep = record.find('\x01');
            if (sep != string::npos) record[sep] = ' ';
            out << " " << record;
        }
    }
    // Сохраняем из второй таблицы
    for (int i = 0; i < 97; i++) {
        if (globalCuckoo.getTable2()->data[i] != "") {
            string record = globalCuckoo.getTable2()->data[i];
            int sep = record.find('\x01');
            if (sep != string::npos) record[sep] = ' ';
            out << " " << record;
        }
    }
    out << "\n";
}

void loadHashes(const string& filename) {
    ifstream f(filename);
    if (!f) return;
 
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tag;
        ss >> tag;
 
        if (tag == "CHAIN") {
            string key, value;
            while (ss >> key >> value) {
                // Восстанавливаем строку с разделителем \x01 
                globalChain.insert(key, value);
            }
        }
        else if (tag == "CUCKOO") {
            string key, value;
            while (ss >> key >> value) {
                globalCuckoo.insert(key, value);
            }
        }
    }
}
