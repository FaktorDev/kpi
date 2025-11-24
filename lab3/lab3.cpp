#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;

struct Stats {
    long long copies = 0; // porivnyannya
    long long porivnyannya = 0;
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
            s.porivnyannya++;

            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                s.copies++;
                newLastSwap = i;
            }
        }

        lastSwap = newLastSwap;
    }
}

#pragma region Сортування за розрядами (Radix Sort, LSD)
int getMax(const vector<int>& a, Stats& s)
{
    int mx = a[0];

    for (int i = 1; i < (int)a.size(); i++)
    {
        s.porivnyannya++;
        if (a[i] > mx)
        {
            mx = a[i];
        }
    }

    return mx;
}

void countingSortByDigit(vector<int>& a, int exp, Stats& s)
{
    int n = (int)a.size();
    vector<int> output(n);
    vector<int> count(10, 0);
    countingSortByDigit;
    // Підрахунок
    for (int i = 0; i < n; i++)
    {
        int digit = (a[i] / exp) % 10;
        count[digit]++;
        s.copies++;
    }

    // Префіксні суми
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Побудова вихідного масиву (порядок стабільності)
    for (int i = n - 1; i >= 0; i--)
    {
        int digit = (a[i] / exp) % 10;
        output[count[digit] - 1] = a[i];
        count[digit]--;
        s.copies++;
    }

    // Копіювання назад
    for (int i = 0; i < n; i++)
    {
        a[i] = output[i];
        s.copies++;
    }
}

void radixSort(vector<int>& a, Stats& s)
{
    if (a.empty()) return;

    int mx = getMax(a, s);

    for (int exp = 1; mx / exp > 0; exp *= 10)
    {
        countingSortByDigit(a, exp, s);
    }
}
#pragma endregion

#pragma region злиття або Merge
void mergeArrays(vector<int>& a, int l, int m, int r, Stats& s)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) {
        L[i] = a[l + i];
        s.copies++;
    }

    for (int i = 0; i < n2; i++) {
        R[i] = a[m + 1 + i];
        s.copies++;
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        s.porivnyannya++;

        if (L[i] <= R[j]) {
            a[k++] = L[i++];
            s.copies++;
        }
        else {
            a[k++] = R[j++];
            s.copies++;
        }
    }

    while (i < n1) {
        a[k++] = L[i++];
        s.copies++;
    }

    while (j < n2) {
        a[k++] = R[j++];
        s.copies++;
    }
}
// злиття
void mergeSort(vector<int>& a, int l, int r, Stats& s, int depth = 1)
{
    if (l >= r) return;

    int m = (l + r) / 2;

    mergeSort(a, l, m, s, depth + 1);
    mergeSort(a, m + 1, r, s, depth + 1);

    mergeArrays(a, l, m, r, s);
}
#pragma endregion

// Generation of arrays
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

// runSort
Stats runBubbleSortLastSwap(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    bubbleSortLastSwap(arr, s);
    return s;
}
Stats runMergeSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    mergeSort(arr, 0,base.size()-1, s);
    return s;
}
Stats runRadixSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    radixSort(arr, s);
    return s;
}

// printTable
void printTable(const vector<int>& sizes,
    const vector<string>& types,
    const vector<vector<Stats>>& results,
    string tableName)
{
    cout << "\n=============================================================\n";
    cout << "                     SORTING RESULTS "<< tableName << endl;
    cout << "=============================================================\n";

    cout << left
        << setw(12) << "N"
        << setw(22) << "Array type"
        << setw(20) << "Porivnyannya"
        << setw(20) << "Copies"
        << "\n";

    cout << "-------------------------------------------------------------\n";

    for (size_t i = 0; i < sizes.size(); i++)
    {
        for (size_t j = 0; j < types.size(); j++)
        {
            cout << left
                << setw(12) << sizes[i]
                << setw(22) << types[j]
                << setw(20) << results[i][j].porivnyannya
                << setw(20) << results[i][j].copies
                << "\n";
        }
        cout << "-------------------------------------------------------------\n";
    }
}

int main()
{
    vector<int> sizes = { 1000, 10000, 100000 };
    vector<string> types = { "Growth", "Random", "Decrease" };

    vector<vector<Stats>> resultsBUBBLE(sizes.size(),
        vector<Stats>(types.size()));
    vector<vector<Stats>> resultsMERGE(sizes.size(),
        vector<Stats>(types.size()));
    vector<vector<Stats>> resultsRADIX(sizes.size(),
        vector<Stats>(types.size()));

    for (size_t i = 0; i < sizes.size(); i++)
    {
        int n = sizes[i];

        // a) Growth
        resultsBUBBLE[i][0] = runBubbleSortLastSwap(generateAscending(n));

        // b) Random
        resultsBUBBLE[i][1] = runBubbleSortLastSwap(generateRandom(n));

        // c) Decrease
        resultsBUBBLE[i][2] = runBubbleSortLastSwap(generateDescending(n));
    }
    for (size_t i = 0; i < sizes.size(); i++)
    {
        int n = sizes[i];

        // a) Growth
        resultsMERGE[i][0] = runMergeSort(generateAscending(n));

        // b) Random
        resultsMERGE[i][1] = runMergeSort(generateRandom(n));

        // c) Decrease
        resultsMERGE[i][2] = runMergeSort(generateDescending(n));
    }
    for (size_t i = 0; i < sizes.size(); i++)
    {
        int n = sizes[i];

        // a) Growth
        resultsRADIX[i][0] = runRadixSort(generateAscending(n));

        // b) Random
        resultsRADIX[i][1] = runRadixSort(generateRandom(n));

        // c) Decrease
        resultsRADIX[i][2] = runRadixSort(generateDescending(n));
    }

    printTable(sizes, types, resultsBUBBLE, "BUBBLE");
    printTable(sizes, types, resultsMERGE, "MERGE");
    printTable(sizes, types, resultsRADIX, "RADIX");

    return 0;
}