#include <iostream>
#include <vector>
using namespace std;

// Шейкерне сортування (cocktail sort) для 1D-масиву
void shakerSort(vector<int>& a) {
    if (a.empty()) return;

    int left = 0;
    int right = (int)a.size() - 1;
    bool swapped = true;

    while (left < right&& swapped) {
        swapped = false;

        // Прохід зліва направо
        for (int i = left; i < right; ++i) {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }
        --right;

        // Прохід справа наліво
        for (int i = right; i > left; --i) {
            if (a[i - 1] > a[i]) {
                swap(a[i - 1], a[i]);
                swapped = true;
            }
        }
        ++left;
    }
}

int main() {
    int n;
    cout << "Введіть розмір квадратної матриці n: ";
    cin >> n;

    vector<vector<int>> a(n, vector<int>(n));

    cout << "Введіть елементи матриці " << n << " x " << n << ":\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> a[i][j];

    // Витягуємо побічну діагональ у окремий масив
    vector<int> diag(n);
    for (int i = 0; i < n; ++i) {
        int j = n - 1 - i;      // індекс стовпця для побічної діагоналі
        diag[i] = a[i][j];
    }

    // Сортуємо побічну діагональ шейкерним сортуванням
    shakerSort(diag);

    // Записуємо відсортовані значення назад у матрицю
    for (int i = 0; i < n; ++i) {
        int j = n - 1 - i;
        a[i][j] = diag[i];
    }

    // Виводимо змінену матрицю
    cout << "Матриця після впорядкування побічної діагоналі:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << a[i][j] << " ";
        cout << "\n";
    }

    return 0;
}
