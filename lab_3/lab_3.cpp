#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;

struct Stats {
	long long comparisons = 0; // porivnyannya
    long long swaps = 0;
};

// Improved Bubble Sort
void bubbleSortLastSwap(vector<int>& a, Stats& s)
{
    int lastSwap = (int)a.size() - 1;

    while (lastSwap > 0)
    {
        int newLastSwap = 0;

        for (int i = 0; i < lastSwap; i++)
        {
            s.comparisons++;

            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                s.swaps++;
                newLastSwap = i;
            }
        }

        lastSwap = newLastSwap;
    }
}

// Генератори масивів
vector<int> generateAscending(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
    return v;
}

vector<int> generateDescending(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = n - i;
    return v;
}

vector<int> generateRandom(int n) {
    vector<int> v(n);
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, 1'000'000);

    for (int i = 0; i < n; i++) v[i] = dist(gen);
    return v;
}

// Запуск тесту
Stats runSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    bubbleSortLastSwap(arr, s);
    return s;
}

// Таблиця результатів
void printTable(const vector<int>& sizes,
    const vector<string>& types,
    const vector<vector<Stats>>& results)
{
    cout << "\n=============================================================\n";
    cout << "                     SORTING RESULTS\n";
    cout << "=============================================================\n";

    cout << left
        << setw(12) << "N"
        << setw(22) << "Array type"
        << setw(20) << "Comparisons"
        << setw(20) << "Exchanges"
        << "\n";

    cout << "-------------------------------------------------------------\n";

    for (size_t i = 0; i < sizes.size(); i++)
    {
        for (size_t j = 0; j < types.size(); j++)
        {
            cout << left
                << setw(12) << sizes[i]
                << setw(22) << types[j]
                << setw(20) << results[i][j].comparisons
                << setw(20) << results[i][j].swaps
                << "\n";
        }
        cout << "-------------------------------------------------------------\n";
    }
}

int main()
{
    vector<int> sizes = { 1000, 10000, 100000 };
    vector<string> types = { "Growth", "Random", "Decrease" };

    vector<vector<Stats>> results(sizes.size(),
        vector<Stats>(types.size()));

    for (size_t i = 0; i < sizes.size(); i++)
    {
        int n = sizes[i];

        // a) зростання
        results[i][0] = runSort(generateAscending(n));

        // b) випадкові
        results[i][1] = runSort(generateRandom(n));

        // c) спадання
        results[i][2] = runSort(generateDescending(n));
    }

    printTable(sizes, types, results);

    return 0;
}
