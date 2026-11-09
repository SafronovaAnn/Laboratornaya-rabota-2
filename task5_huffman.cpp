#include "task5_huffman.h"
#include <cstdint>   
#include <utility>   

// Вспомогательные функции

// Рекурсивная генерация кодов (обход дерева)
void GenerateCodes(HNode* root, string code, map<char, string>& huffmanCode) {
    if (!root) return;

    // Если это лист (нет детей) и это символ
    if (!root->left && !root->right && root->ch != '\0') {
        huffmanCode[root->ch] = code.empty() ? "0" : code; 
    }

    GenerateCodes(root->left,  code + "0", huffmanCode);
    GenerateCodes(root->right, code + "1", huffmanCode);
}

// Освобождение памяти дерева
void FreeTree(HNode* root) {
    if (!root) return;
    FreeTree(root->left);
    FreeTree(root->right);
    delete root;
}

// Раскодирование

string DecodeHuffman(HNode* root, const string& encoded) {
    string result = "";

    // Если в дереве только один узел (один уникальный символ)
    if (root && !root->left && !root->right) {
        for (size_t i = 0; i < encoded.length(); i++) {
            result += root->ch;
        }
        return result;
    }
    
    HNode* current = root;
    
    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        // Если дошли до листа (символ)
        if (!current->left && !current->right) {
            result += current->ch;
            current = root; // Возвращаемся в корень для чтения следующего символа
        }
    }
    return result;
}

// Основная функция

void RunHuffman(const string& text) {
    if (text.empty()) return;

    // Разделитель: непечатный символ SOH ('\x01')
    const char SEP = '\x01';

    // Считаем частоты
    map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // Кладём в массив 
    Array arr;
    Init(&arr);

    for (auto& pair : freqMap) {
        HNode* node = new HNode{pair.first, pair.second, nullptr, nullptr};
        string record = to_string(pair.second) + SEP + to_string((uintptr_t)node);
        pushBack(&arr, record);
    }

    // Строим дерево
    while (arr.size > 1) {
        // Ищем два минимума по полю freq
        int min1 = 0, min2 = 1;

        int f1 = stoi(arr.data[min1].substr(0, arr.data[min1].find(SEP)));
        int f2 = stoi(arr.data[min2].substr(0, arr.data[min2].find(SEP)));
        if (f1 > f2) swap(min1, min2);

        for (int i = 2; i < arr.size; i++) {
            int f  = stoi(arr.data[i].substr(0, arr.data[i].find(SEP)));
            int fa = stoi(arr.data[min1].substr(0, arr.data[min1].find(SEP)));
            int fb = stoi(arr.data[min2].substr(0, arr.data[min2].find(SEP)));

            if (f < fa) {
                min2 = min1;
                min1 = i;
            } else if (f < fb) {
                min2 = i;
            }
        }

        // Распаковываем адреса узлов
        string r1 = arr.data[min1];
        string r2 = arr.data[min2];
        HNode* left  = (HNode*)(uintptr_t)stoull(r1.substr(r1.find(SEP) + 1));
        HNode* right = (HNode*)(uintptr_t)stoull(r2.substr(r2.find(SEP) + 1));

        // Создаём родителя
        HNode* parent = new HNode{'\0', left->freq + right->freq, left, right};

        // Удаляем два минимальных
        if (min1 > min2) {
            del(&arr, min1);
            del(&arr, min2);
        } else {
            del(&arr, min2);
            del(&arr, min1);
        }

        // Добавляем родителя
        string parentRecord = to_string(parent->freq) + SEP + to_string((uintptr_t)parent);
        pushBack(&arr, parentRecord);
    }

    // Корень
    string rootRec = arr.data[0];
    HNode* root = (HNode*)(uintptr_t)stoull(rootRec.substr(rootRec.find(SEP) + 1));

    // Генерируем коды
    map<char, string> huffmanCode;
    GenerateCodes(root, "", huffmanCode);

    // Выводим таблицу 
    DList table;
    Init(&table);
    for (auto& pair : huffmanCode) {
        pushBack(&table, string(1, pair.first) + " : " + pair.second);
    }
    PrintForward(&table);

    // Кодируем строку
    string encoded;
    for (char ch : text) {
        encoded += huffmanCode[ch];
    }
    cout << "\nЗакодированная строка:\n" << encoded << endl;

    // Раскодируем обратно 
    string decoded = DecodeHuffman(root, encoded);
    cout << "\nРаскодированная строка:\n" << decoded << endl;

    // Очистка памяти
    FreeTree(root);
    Free(&arr);

    while (table.head) {
        DNode* n = table.head;
        table.head = table.head->next;
        delete n;
    }
}