#include "task3_subsets.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void PartitionSubsets(const string& numbers_str) {
    MySet inputSet;
    
    // Парсим строку и добавляем в множество
    stringstream ss(numbers_str);
    string item;
    while (getline(ss, item, ',')) {
        inputSet.add(item); 
    }

    // Переносим уникальные элементы из множества в вектор для алгоритма
    vector<int> nums;
    int totalSum = 0;
    
    // Идем по всем бакетам множества
    for (int i = 0; i < MySet::TABLE_SIZE; i++) {
        DNode* cur = inputSet.buckets[i].head; 
        while (cur) {
            int val = stoi(cur->data);
            
            // Защита от отрицательных чисел
            if (val < 0) {
                cerr << "Ошибка: все числа должны быть натуральными (положительными)" << endl;
                return;
            }

            nums.push_back(val);
            totalSum += val;
            cur = cur->next;
        }
    }

    // Проверка на пустоту
    if (nums.empty()) {
        cerr << "Ошибка: пустой ввод" << endl;
        return;
    }

    // Алгоритм 
    int target = totalSum / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int x : nums) {
        for (int i = target; i >= x; i--) {
            if (dp[i - x]) dp[i] = true;
        }
    }

    // Ищем максимально достижимую сумму
    int sum1 = 0;
    for (int i = target; i >= 0; i--) {
        if (dp[i]) { 
            sum1 = i; 
            break; 
        }
    }

    int sum2 = totalSum - sum1;
    int diff = sum2 - sum1;

    // Восстанавливаем, какие числа попали в первую группу
    vector<int> group1, group2;
    int currentTarget = sum1;

    for (int i = nums.size() - 1; i >= 0; i--) {
        int x = nums[i];
        if (currentTarget >= x && dp[currentTarget - x]) {
            group1.push_back(x);
            currentTarget -= x;
        } else {
            group2.push_back(x);
        }
    }

    // Вывод результата
    sort(group1.begin(), group1.end());
    sort(group2.begin(), group2.end());

    cout << "Подмножество 1: {";
    for (size_t i = 0; i < group1.size(); i++) {
        cout << group1[i] << (i + 1 < group1.size() ? ", " : "");
    }
    cout << "}" << endl;

    cout << "Подмножество 2: {";
    for (size_t i = 0; i < group2.size(); i++) {
        cout << group2[i] << (i + 1 < group2.size() ? ", " : "");
    }
    cout << "}" << endl;
    
    cout << "Разница между суммами: " << diff << endl;
}