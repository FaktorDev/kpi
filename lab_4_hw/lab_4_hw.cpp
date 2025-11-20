#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    int n, m;
    cout << "Введіть n (розмір A): ";
    cin >> n;
    cout << "Введіть m (розмір B, m < n): ";
    cin >> m;

    // Виділення памʼяті під масиви
    int* A = new int[n];
    int* B = new int[m];

    // Генерація A
    for (int i = 0; i < n; i++)
        A[i] = rand() % 10;

    // Генерація B
    for (int i = 0; i < m; i++)
        B[i] = rand() % 10;

    // Вивід
    cout << "A: ";
    for (int i = 0; i < n; i++)
        cout << A[i] << " ";
    cout << endl;

    cout << "B: ";
    for (int i = 0; i < m; i++)
        cout << B[i] << " ";
    cout << endl;

    // Пошук входження B у A
    int start = -1;

    for (int i = 0; i <= n - m; i++) {
        bool match = true;

        for (int j = 0; j < m; j++) {
            if (A[i + j] != B[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        cout << "Підпослідовність B не знайдена в A" << endl;

        delete[] A;
        delete[] B;
        return 0;
    }

    // Видалення підпослідовності (ручний зсув елементів)
    for (int i = start; i < n - m; i++) {
        A[i] = A[i + m];
    }

    // новий розмір масиву після видалення
    int newSize = n - m;

    cout << "A після видалення підпослідовності B: ";
    for (int i = 0; i < newSize; i++)
        cout << A[i] << " ";
    cout << endl;

    // Звільнення памʼяті
    // Звільнення памʼяті
    delete[] A;
    delete[] B;

    return 0;
}
