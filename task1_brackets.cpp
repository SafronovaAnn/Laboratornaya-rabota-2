#include "task1_brackets.h"

string RemovePar(const string& s) {
    Stack st;
    Init(&st);

    vector<bool> keep(s.length(), true);

    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '(') {
            push(&st, to_string(i));
        }
        else if (s[i] == ')') {
            if (!isEmpty(&st)) {
                SNode* node = pop(&st);   
                delete node;              
            } else {
                keep[i] = false;         
            }
        }
    }

    while (!isEmpty(&st)) {
        SNode* node = pop(&st);
        int index = stoi(node->data);
        keep[index] = false;
        delete node;
    }

    // Собираем результат
    string result;
    result.reserve(s.length());
    for (int i = 0; i < (int)s.length(); i++) {
        if (keep[i]) result += s[i];
    }

    return result;
}