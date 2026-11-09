#ifndef TASK5_HUFFMAN_H
#define TASK5_HUFFMAN_H

#include <iostream>
#include <string>
#include <map>
#include "array.h"        
#include "double_list.h"  

using namespace std;

// Узел дерева Хаффмана
struct HNode {
    char ch;          // символ '\0' для внутреннего узла
    int freq;         // частота
    HNode* left;
    HNode* right;
};

void RunHuffman(const string& text);

string DecodeHuffman(HNode* root, const string& encoded);

#endif