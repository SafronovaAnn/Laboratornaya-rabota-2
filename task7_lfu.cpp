#include "task7_lfu.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

LFUCache::LFUCache(int cap) {
    capacity = cap;
    Init(&list);
}

LFUCache::~LFUCache() {
    DNode* cur = list.head;
    while (cur) {
        DNode* next = cur->next;
        int sep = cur->data.find('\x01');
        string addrStr = cur->data.substr(sep + 1);
        LFUNode* node = (LFUNode*)(uintptr_t)stoull(addrStr);
        delete node;
        cur = next;
    }
}

LFUNode* LFUCache::findNode(const string& key) {
    string val = chain.find(key);
    if (val != "") return (LFUNode*)(uintptr_t)stoull(val);
    
    val = cuckoo.find(key);
    if (val != "") return (LFUNode*)(uintptr_t)stoull(val);
    
    return nullptr;
}

int LFUCache::get(int key) {
    string keyStr = to_string(key);
    LFUNode* node = findNode(keyStr);
    if (!node) return -1;
    
    node->freq++; 
    int colon = node->data.find(':');
    return stoi(node->data.substr(colon + 1));
}

// Создание узла (без добавления в таблицы)
LFUNode* LFUCache::createNode(int key, int value) {
    string keyStr = to_string(key);
    LFUNode* node = findNode(keyStr);

    if (node) {
        node->data = keyStr + ":" + to_string(value);
        node->freq++;
        return node; // Возвращаем существующий узел
    }

    int currentSize = 0;
    DNode* tmp = list.head;
    while (tmp) { currentSize++; tmp = tmp->next; }

    if (currentSize >= capacity) {
        // Ищем элемент с минимальной частотой (самый старый)
        int minFreq = 999999;
        DNode* minNode = nullptr;
        LFUNode* minLFU = nullptr;

        DNode* cur = list.tail;
        while (cur) {
            int sep = cur->data.find('\x01');
            string addrStr = cur->data.substr(sep + 1);
            LFUNode* lfu = (LFUNode*)(uintptr_t)stoull(addrStr);
            if (lfu->freq < minFreq) {
                minFreq = lfu->freq;
                minNode = cur;
                minLFU = lfu;
            }
            cur = cur->prev;
        }

        string oldKey = minLFU->data.substr(0, minLFU->data.find(':'));
        
        // Удаляем из таблицы
        if (minLFU->isChain) {
            chain.remove(oldKey);
        } else {
            cuckoo.remove(oldKey);
        }

        // Удаляем из списка
        if (minNode->prev) minNode->prev->next = minNode->next;
        if (minNode->next) minNode->next->prev = minNode->prev;
        if (minNode == list.head) list.head = minNode->next;
        if (minNode == list.tail) list.tail = minNode->prev;
        delete minLFU;
        delete minNode;
    }

    LFUNode* newNode = new LFUNode();
    newNode->data = keyStr + ":" + to_string(value);
    newNode->freq = 1;
    newNode->isChain = true;
    DNode* listNode = new DNode();
    listNode->data = keyStr + '\x01' + to_string((uintptr_t)newNode);
    listNode->prev = nullptr;
    listNode->next = list.head;

    if (list.head) list.head->prev = listNode;
    list.head = listNode;
    if (!list.tail) list.tail = listNode;
    newNode->listNode = listNode;

    return newNode; 
}

void LFUCache::putInChain(int key, int value) {
    LFUNode* node = createNode(key, value);
    if (node) {
        string keyStr = to_string(key);
        cuckoo.remove(keyStr); // Чистим другую таблицу
        
        node->isChain = true;
        string addrStr = to_string((uintptr_t)node);
        chain.insert(keyStr, addrStr);
    }
}

void LFUCache::putInCuckoo(int key, int value) {
    LFUNode* node = createNode(key, value);
    if (node) {
        string keyStr = to_string(key);
        chain.remove(keyStr); // Чистим другую таблицу
        
        node->isChain = false;
        string addrStr = to_string((uintptr_t)node);
        cuckoo.insert(keyStr, addrStr);
    }
}

void LFUCache::setCapacity(int newCap) {
    capacity = newCap;
    DNode* cur = list.head;
    while (cur) {
        DNode* next = cur->next;
        int sep = cur->data.find('\x01');
        string addrStr = cur->data.substr(sep + 1);
        LFUNode* node = (LFUNode*)(uintptr_t)stoull(addrStr);
        delete node;
        delete cur;
        cur = next;
    }
    Init(&list);
    // Очищаем таблицы
    for (int i = 0; i < 97; i++) {
        chain.getBuckets()[i].head = nullptr;
        chain.getBuckets()[i].tail = nullptr;
    }
    for (int i = 0; i < 97; i++) {
        set(cuckoo.getTable1(), i, "");
        set(cuckoo.getTable2(), i, "");
    }
}

void LFUCache::printCache() {
    if (!list.head) {
        cout << "Cache: (пусто)" << endl;
        return;
    }
    cout << "Cache: ";
    DNode* cur = list.head;
    while (cur) {
        int sep = cur->data.find('\x01');
        string addrStr = cur->data.substr(sep + 1);
        LFUNode* lfu = (LFUNode*)(uintptr_t)stoull(addrStr);
        cout << "[" << lfu->data << " freq=" << lfu->freq << "] ";
        cur = cur->next;
    }
    cout << endl;
}

LFUCache globalLFU(3);

void saveLFU(const string& filename) {
    ifstream in(filename);
    vector<string> lines;
    string line;
    while (getline(in, line)) {
        if (line.find("LFU ") != 0) lines.push_back(line);
    }
    in.close();

    ofstream out(filename);
    for (const string& l : lines) out << l << "\n";

    out << "LFU " << globalLFU.getCapacity();
    
    // Сохраняем от tail к head
    DNode* cur = globalLFU.getList()->tail;
    while (cur) {
        int sep = cur->data.find('\x01');
        string addrStr = cur->data.substr(sep + 1);
        LFUNode* lfu = (LFUNode*)(uintptr_t)stoull(addrStr);
        
        string table = lfu->isChain ? "C" : "K"; 
        
        out << " " << lfu->data << "#" << lfu->freq << "#" << table;
        cur = cur->prev;
    }
    out << "\n";
}

void loadLFU(const string& filename) {
    ifstream f(filename);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string tag;
        ss >> tag;

        if (tag == "LFU") {
            int cap;
            ss >> cap;
            globalLFU.setCapacity(cap);
            
            vector<string> items;
            string item;
            while (ss >> item) items.push_back(item);

            // Восстанавливаем порядок
            for (int i = 0; i < (int)items.size(); i++) {
                string data = items[i];
                int hash1 = data.find('#');
                int hash2 = data.find('#', hash1 + 1);
                
                string kv = data.substr(0, hash1);
                int freq = stoi(data.substr(hash1 + 1, hash2 - hash1 - 1));
                string table = data.substr(hash2 + 1);
                
                int colon = kv.find(':');
                int key = stoi(kv.substr(0, colon));
                int val = stoi(kv.substr(colon + 1));
                string keyStr = to_string(key);

                LFUNode* newNode = new LFUNode();
                newNode->data = keyStr + ":" + to_string(val);
                newNode->freq = freq;
                newNode->isChain = (table == "C");

                DNode* listNode = new DNode();
                listNode->data = keyStr + '\x01' + to_string((uintptr_t)newNode);
                listNode->prev = nullptr;
                listNode->next = globalLFU.getList()->head;

                if (globalLFU.getList()->head) globalLFU.getList()->head->prev = listNode;
                globalLFU.getList()->head = listNode;
                if (!globalLFU.getList()->tail) globalLFU.getList()->tail = listNode;

                newNode->listNode = listNode;

                string addrStr = to_string((uintptr_t)newNode);
                if (newNode->isChain) {
                    globalLFU.getChain()->insert(keyStr, addrStr);
                } else {
                    globalLFU.getCuckoo()->insert(keyStr, addrStr);
                }
            }
        }
    }
}