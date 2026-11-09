#include "task4_array.h"

void FindSubarraysWithSum(Array* arr, int target) {
    if (arr->size == 0) {
        cerr << "Ошибка: массив пуст" << endl;
        return;
    }

    bool found = false;

    // Начало подмассива
    for (int start = 0; start < arr->size; start++) {
        int currentSum = 0;

        // Конец подмассива
        for (int end = start; end < arr->size; end++) {
            currentSum += stoi(arr->data[end]);

            if (currentSum == target) {
                found = true;
                cout << "[";
                for (int i = start; i <= end; i++) {
                    cout << arr->data[i];
                    if (i < end) cout << ", ";
                }
                cout << "]" << endl;
            }
        }
    }

    if (!found) {
        cout << "Подмассивов с суммой " << target << " не найдено." << endl;
    }
}