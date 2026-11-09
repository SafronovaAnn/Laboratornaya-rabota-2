#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

#include "task1_brackets.h"
#include "task2_set.h"
#include "task3_subsets.h"
#include "task4_array.h"
#include "task5_huffman.h"
#include "task6_1_hash.h"
#include "task6_2_rehash.h"
#include "task7_lfu.h"

using namespace std;

// Вспомогательная функция для разбиения строки на слова
vector<string> split(const string& s) {
    vector<string> result;
    stringstream ss(s);
    string token;
    while (ss >> token) result.push_back(token);
    return result;
}

void executeQuery(const string& query) {
    const int TEST_SIZE = 1000;
    vector<string> t = split(query);
    if (t.empty()) return;

    string cmd = t[0];


    // Задание 1: Скобки
    if (cmd == "FIXBRACKETS" && t.size() >= 2) {
        string input_str = t[1];
        for (size_t i = 2; i < t.size(); i++) input_str += " " + t[i];
        cout << RemovePar(input_str) << endl;
        return;
    }

    // Задание 2: Множество
    if (cmd == "SETADD" && t.size() == 3) {
        globalSets[t[1]].add(t[2]);
        return;
    }
    if (cmd == "SETDEL" && t.size() == 3) {
        auto it = globalSets.find(t[1]);
        if (it != globalSets.end()) it->second.remove(t[2]);
        return;
    }
    if (cmd == "SET_AT" && t.size() == 3) {
        auto it = globalSets.find(t[1]);
        bool found = (it != globalSets.end() && it->second.contains(t[2]));
        cout << (found ? "TRUE" : "FALSE") << endl;
        return;
    }
    if (cmd == "SETPRINT" && t.size() == 2) {
        auto it = globalSets.find(t[1]);
        if (it != globalSets.end()) it->second.print();
        else cerr << "Ошибка: множество не найдено" << endl;
        return;
    }

    // Задание 3: Разбиение 
    if (cmd == "PARTITION" && t.size() == 2) {
        PartitionSubsets(t[1]);
        return;
    }

    // Задание 4: Подмассив с суммой
    if (cmd == "FINDSUM" && t.size() == 3) {
        Array tempArr;
        Init(&tempArr);
        stringstream ss(t[1]);
        string item;
        while (getline(ss, item, ',')) pushBack(&tempArr, item);
        int target = stoi(t[2]);
        FindSubarraysWithSum(&tempArr, target);
        Free(&tempArr);
        return;
    }

    // Задание 5: Хаффман
    if (cmd == "HUFFMAN" && t.size() >= 2) {
        string input_str = t[1];
        for (size_t i = 2; i < t.size(); i++) input_str += " " + t[i];
        RunHuffman(input_str);
        return;
    }

    // Задание 6.1: Хеш-таблицы (Цепочки / Кукушка)
    if (cmd == "CHAINADD" && t.size() == 3) {
        globalChain.insert(t[1], t[2]); 
        return;
    }
    if (cmd == "CHAINFIND" && t.size() == 2) {
        string val = globalChain.find(t[1]);
        if (val == "") cout << "NOT FOUND" << endl;
        else cout << "Значение: " << val << endl;
        return;
    }
    if (cmd == "CHAINDEL" && t.size() == 2) {
        globalChain.remove(t[1]);
        return;
    }
    if (cmd == "CHAINPRINT") {
        globalChain.print();
        return;
    }
 
    // Задание 6.1: Кукушка
    if (cmd == "CUCKOOADD" && t.size() == 3) {
        globalCuckoo.insert(t[1], t[2]); 
        return;
    }
    if (cmd == "CUCKOOFIND" && t.size() == 2) {
        string val = globalCuckoo.find(t[1]);
        if (val == "") cout << "NOT FOUND" << endl;
        else cout << "Значение: " << val << endl;
        return;
    }
    if (cmd == "CUCKOOPRINT") {
        globalCuckoo.print();
        return;
    }
    if (cmd == "CUCKOODEL" && t.size() == 2) {
        globalCuckoo.remove(t[1]);
        return;
    }


    // Задание 6.2: Реструктуризация 
    if (cmd == "REHASHADD" && t.size() == 3) {
        globalRehash.insert(t[1], t[2]);
        return;
    }
    if (cmd == "REHASHFIND" && t.size() == 2) {
        string val = globalRehash.find(t[1]);
        if (val == "") cout << "NOT FOUND" << endl;
        else cout << "Значение: " << val << endl;
        return;
    }
    if (cmd == "REHASHDEL" && t.size() == 2) {
        globalRehash.remove(t[1]);
        return;
    }
    if (cmd == "REHASHPRINT") {
        globalRehash.print();
        return;
    }

    // Задание 7: LFU
    if (cmd == "LFUSETCAP" && t.size() == 2) {
        globalLFU.setCapacity(stoi(t[1]));
        return;
    }
    if (cmd == "LFUCHAIN" && t.size() == 3) {
        globalLFU.putInChain(stoi(t[1]), stoi(t[2]));
        return;
    }
    if (cmd == "LFUCUCKOO" && t.size() == 3) {
        globalLFU.putInCuckoo(stoi(t[1]), stoi(t[2]));
        return;
    }
    if (cmd == "LFUGET" && t.size() == 2) {
        cout << globalLFU.get(stoi(t[1])) << endl; 
        return;
    }
    if (cmd == "LFUPRINT") {
        globalLFU.printCache(); 
        return;
    }
   
    if (cmd == "TEST") { 
        cout << " Эмпирический анализ " << endl; 
        cout << " Для " << TEST_SIZE << " элементов " << endl; 

           // 1. Замер метода цепочек
    clock_t start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalChain.insert("key" + to_string(i), "val" + to_string(i));
    }
    clock_t end = clock();
    double time_chain_insert = double(end - start) / CLOCKS_PER_SEC;
    
    // Поиск
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalChain.find("key" + to_string(i));
    }
    end = clock();
    double time_chain_find = double(end - start) / CLOCKS_PER_SEC;
    
    // Удаление 
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalChain.remove("key" + to_string(i));
    }
    end = clock();
    double time_chain_del = double(end - start) / CLOCKS_PER_SEC;

    cout << "--- Метод цепочек ---" << endl;
    cout << "Вставка:  " << time_chain_insert << " сек" << endl;
    cout << "Поиск:    " << time_chain_find << " сек" << endl;
    cout << "Удаление: " << time_chain_del << " сек" << endl;

    // 2. Замер кукушки
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalCuckoo.insert("key" + to_string(i), "val" + to_string(i));
    }
    end = clock();
    double time_cuckoo_insert = double(end - start) / CLOCKS_PER_SEC;

    // Поиск 
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalCuckoo.find("key" + to_string(i));
    }
    end = clock();
    double time_cuckoo_find = double(end - start) / CLOCKS_PER_SEC;

    // Удаление 
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        globalCuckoo.remove("key" + to_string(i));
    }
    end = clock();
    double time_cuckoo_del = double(end - start) / CLOCKS_PER_SEC;

    cout << "--- Хеширование кукушки ---" << endl;
    cout << "Вставка: " << time_cuckoo_insert << " сек" << endl;
    cout << "Поиск: " << time_cuckoo_find << " сек" << endl;
    cout << "Удаление: " << time_cuckoo_del << " сек" << endl;
    
    return;
}
    cerr << "Ошибка: неизвестная команда '" << cmd << "'" << endl;
}

int main(int argc, char* argv[]) {
    string filename, query;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filename = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    if (filename.empty() || query.empty()) {
        cerr << "Использование: ./dbms --file <файл> --query \"<команда>\"" << endl;
        return 1;
    }

    // 1. Загружаем всё из файла
    loadSetsFromFile(filename);
    loadHashes(filename);
    loadRehash(filename);
    loadLFU(filename);

    // 2. Выполняем команду
    executeQuery(query);

    // 3. Сохраняем, если команда меняла данные
    if (query.find("SETADD") == 0 || query.find("SETDEL") == 0) {
        saveSetsToFile(filename);
    }
    
    // Хеш-таблицы (Задание 6.1)
    if (query.find("CHAINADD") == 0 || query.find("CHAINDEL") == 0 || 
    query.find("CUCKOOADD") == 0 || query.find("CUCKOODEL") == 0) { 
    saveHashes(filename);
}
    
    // Реструктуризация (Задание 6.2)
    if (query.find("REHASHADD") == 0 || query.find("REHASHDEL") == 0) {
        saveRehash(filename);
    }
    
    // LRU-кэш (Задание 7) — сохраняем и после GET
    if (query.find("LFUPUT") == 0 || query.find("LFUGET") == 0 || 
        query.find("LFUCHAIN") == 0 || query.find("LFUCUCKOO") == 0 || 
        query.find("LFUSETCAP") == 0) {
        saveLFU(filename);
    }
    
    return 0;
}