#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <windows.h>

using namespace std;


struct Stats {
	long long porivnyannya = 0; // porivnyannya
    long long copies = 0;

    long long timeMs = 0;
    long long recursionCalls = 0;
    long long maxRecursionDepth = 0;
    long long memoryAllocated = 0;
};




#pragma region обмін, із фіксацією останнього місця пересування
// обмін, із фіксацією останнього місця пересування
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
#pragma endregion

#pragma region злиття або Merge
void mergeArrays(vector<int>& a, int l, int m, int r, Stats& s)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);
    s.memoryAllocated += sizeof(int) * (n1 + n2);
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
    s.recursionCalls++;
    s.maxRecursionDepth = max(s.maxRecursionDepth, (long long)depth);

    if (l >= r) return;

    int m = (l + r) / 2;

    mergeSort(a, l, m, s, depth + 1);
    mergeSort(a, m + 1, r, s, depth + 1);

    mergeArrays(a, l, m, r, s);
}
#pragma endregion

#pragma region підрахунком або Counting
// підрахунком
void countingSort(vector<int>& a, Stats& s)
{
    if (a.empty()) return;

    int mn = a[0], mx = a[0];

    // Знаходимо діапазон
    for (int i = 1; i < (int)a.size(); i++) {
        s.porivnyannya++;
        if (a[i] < mn) mn = a[i];
        s.porivnyannya++;
        if (a[i] > mx) mx = a[i];
    }

    int range = mx - mn + 1;
    vector<int> count(range, 0);
    s.memoryAllocated += sizeof(int) * range;
    // Підрахунок
    for (int x : a) {
        count[x - mn]++;
        s.copies++;
    }

    // Відновлення масиву
    int idx = 0;
    for (int i = 0; i < range; i++)
    {
        while (count[i] > 0)
        {
            a[idx++] = i + mn;
            count[i]--;
            s.copies++;
        }
    }
}
#pragma endregion

#pragma region вставка або Insertion
// вставка
void insertionSort(vector<int>& a, Stats& s)
{
    int n = (int)a.size();

    for (int i = 1; i < n; i++)
    {
        int key = a[i];
        s.copies++;

        int j = i - 1;

        while (j >= 0)
        {
            s.porivnyannya++;

            if (a[j] > key)
            {
                a[j + 1] = a[j];
                s.copies++;
                j--;
            }
            else
            {
                break;
            }
        }

        a[j + 1] = key;
        s.copies++;
    }
}
#pragma endregion

#pragma region вибір або Selection
// вибір
void selectionSort(vector<int>& a, Stats& s)
{
    int n = (int)a.size();

    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < n; j++)
        {
            s.porivnyannya++;

            if (a[j] < a[minIndex])
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            swap(a[i], a[minIndex]);
            s.copies++;
        }
    }
}
#pragma endregion

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

#pragma region Коміркове сортування (Bucket Sort)
void insertionSortLocal(vector<int>& a, Stats& s)
{
    for (int i = 1; i < (int)a.size(); i++)
    {
        int key = a[i];
        s.copies++;

        int j = i - 1;

        while (j >= 0)
        {
            s.porivnyannya++;
            if (a[j] > key)
            {
                a[j + 1] = a[j];
                s.copies++;
                j--;
            }
            else
                break;
        }

        a[j + 1] = key;
        s.copies++;
    }
}

void bucketSort(vector<int>& a, Stats& s)
{
    int n = (int)a.size();
    if (n == 0) return;

    // Шукаємо мінімум та максимум
    int mn = a[0], mx = a[0];
    for (int i = 1; i < n; i++)
    {
        s.porivnyannya++;
        if (a[i] < mn) mn = a[i];

        s.porivnyannya++;
        if (a[i] > mx) mx = a[i];
    }

    int bucketCount = max(1, (int)sqrt(n));
    vector<vector<int>> buckets(bucketCount);
    s.memoryAllocated += sizeof(vector<int>) * bucketCount;
    double range = (double)(mx - mn + 1);
    double step = range / bucketCount;

    // Заповнення комірок
    for (int i = 0; i < n; i++)
    {
        int index = (int)((a[i] - mn) / step);
        if (index == bucketCount) index--; // Корекція краю

        buckets[index].push_back(a[i]);
        s.copies++;
    }

    // Сортування кожної комірки
    for (int i = 0; i < bucketCount; i++)
    {
        if (!buckets[i].empty())
            insertionSortLocal(buckets[i], s);
    }

    // Переклад назад у масив
    int k = 0;
    for (int i = 0; i < bucketCount; i++)
    {
        for (int x : buckets[i])
        {
            a[k++] = x;
            s.copies++;
        }
    }
}
#pragma endregion

#pragma region Пірамідальне сортування (Heap Sort)
void heapify(vector<int>& a, int n, int i, Stats& s)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Лівий син
    if (left < n)
    {
        s.porivnyannya++;
        if (a[left] > a[largest])
            largest = left;
    }

    // Правий син
    if (right < n)
    {
        s.porivnyannya++;
        if (a[right] > a[largest])
            largest = right;
    }

    // Якщо найбільший не батько – міняємо
    if (largest != i)
    {
        swap(a[i], a[largest]);
        s.copies++;

        heapify(a, n, largest, s);
    }
}

void heapSort(vector<int>& a, Stats& s)
{
    int n = (int)a.size();
    if (n == 0) return;

    // Побудова купи
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i, s);

    // Один за одним витягуємо максимум
    for (int i = n - 1; i > 0; i--)
    {
        swap(a[0], a[i]);
        s.copies++;

        heapify(a, i, 0, s);
    }
}
#pragma endregion

#pragma region Звичайне обмінне сортування (Bubble Sort)
void bubbleSort(vector<int>& a, Stats& s)
{
    int n = (int)a.size();

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            s.porivnyannya++;

            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                s.copies++;
            }
        }
    }
}
#pragma endregion

#pragma region Обмін, із фіксацією наявності пересувань (Bubble Sort with flag)
void bubbleSortFlag(vector<int>& a, Stats& s)
{
    int n = (int)a.size();

    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++)
        {
            s.porivnyannya++;

            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                s.copies++;
                swapped = true;
            }
        }

        if (!swapped)
            break; // масив уже відсортований
    }
}
#pragma endregion

#pragma region Швидке сортування (QuickSort)
int partitionQS(vector<int>& a, int low, int high, Stats& s)
{
    int pivot = a[(low + high) / 2]; // опорний елемент
    s.copies++;

    int i = low - 1;
    int j = high + 1;

    while (true)
    {
        // Рухаємо правий покажчик вліво
        do {
            j--;
            s.porivnyannya++;
        } while (a[j] > pivot);

        // Рухаємо лівий покажчик вправо
        do {
            i++;
            s.porivnyannya++;
        } while (a[i] < pivot);

        if (i < j)
        {
            swap(a[i], a[j]);
            s.copies++;
        }
        else
        {
            return j;
        }
    }
}

void quickSort(vector<int>& a, int low, int high, Stats& s, int depth = 1)
{
    s.recursionCalls++;
    s.maxRecursionDepth = max(s.maxRecursionDepth, (long long)depth);

    if (low < high)
    {
        int p = partitionQS(a, low, high, s);

        quickSort(a, low, p, s, depth + 1);
        quickSort(a, p + 1, high, s, depth + 1);
    }
}
#pragma endregion




#pragma region Шейкерне сортування (Shaker Sort)
void shakerSort(vector<int>& a, Stats& s)
{
    int left = 0;
    int right = (int)a.size() - 1;

    while (left < right)
    {
        int newRight = left;

        // Прохід вправо
        for (int i = left; i < right; i++)
        {
            s.porivnyannya++;
            if (a[i] > a[i + 1])
            {
                swap(a[i], a[i + 1]);
                s.copies++;
                newRight = i;
            }
        }

        right = newRight;

        int newLeft = right;

        // Прохід вліво
        for (int i = right; i > left; i--)
        {
            s.porivnyannya++;
            if (a[i - 1] > a[i])
            {
                swap(a[i - 1], a[i]);
                s.copies++;
                newLeft = i;
            }
        }

        left = newLeft;
    }
}
#pragma endregion

#pragma region Запуски тестів
Stats runBubbleSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    bubbleSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runBubbleSortFlag(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    bubbleSortFlag(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runBubbleSortLastSwap(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    bubbleSortLastSwap(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runShakerSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    shakerSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runSelectionSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    selectionSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runMergeSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    mergeSort(arr, 0, base.size() - 1, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runCountingSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    countingSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runRadixSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    radixSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}

Stats runBucketSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    bucketSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}

Stats runHeapSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    heapSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runInsertionSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;
    auto start = chrono::high_resolution_clock::now();
    insertionSort(arr, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
Stats runQuickSort(const vector<int>& base)
{
    vector<int> arr = base;
    Stats s;

    auto start = chrono::high_resolution_clock::now();
    if (!arr.empty())
        quickSort(arr, 0, arr.size() - 1, s);
    auto end = chrono::high_resolution_clock::now();

    s.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return s;
}
#pragma endregion

#pragma region Генератори масивів
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
#pragma endregion

void printTable(const vector<int>& sizes,
    const vector<string>& types,
    const vector<vector<Stats>>& results,
    string tableName)
{
    cout << "\n=============================================================\n";
    cout << "                     SORTING RESULTS "<<tableName<<endl;
    cout << "=============================================================\n";

    cout << left
        << setw(8) << "N"
        << setw(12) << "Type"
        << setw(14) << "Cmp"
        << setw(14) << "Copies"
        << setw(10) << "Time(ms)"
        << setw(12) << "RecCalls"
        << setw(12) << "MaxDepth"
        << setw(14) << "Mem(B)"
        << "\n";

    cout << "-------------------------------------------------------------\n";

    for (size_t i = 0; i < sizes.size(); i++)
    {
        for (size_t j = 0; j < types.size(); j++)
        {
            const Stats& st = results[i][j];
            cout << left
                << setw(8) << sizes[i]
                << setw(12) << types[j]
                << setw(14) << st.porivnyannya
                << setw(14) << st.copies
                << setw(10) << st.timeMs
                << setw(12) << st.recursionCalls
                << setw(12) << st.maxRecursionDepth
                << setw(14) << st.memoryAllocated
                << "\n";
        }
        cout << "-------------------------------------------------------------\n";
    }
}

//=====================
// Структура алгоритму
//=====================
struct Algo {
    string name;
    Stats(*run)(const vector<int>&);
};

struct ProgressState {
    int percent;
    string name;
};
vector<ProgressState> progressBars;
mutex coutMutex;
bool progressRunning = true;

void progressDisplayThread()
{
    while (progressRunning)
    {
        {
            lock_guard<mutex> lock(coutMutex);

            cout << "\033[H\033[J"; // очистити екран

            cout << "Algorithm progress:\n\n";

            for (auto& p : progressBars)
            {
                int per = p.percent;
                int barWidth = 30;
                int filled = (barWidth * per) / 100;

                cout << "[" << setw(14) << left << p.name << "]  ";

                for (int i = 0; i < filled; i++) cout << "#";
                for (int i = filled; i < barWidth; i++) cout << ".";

                cout << "  " << setw(3) << per << "%\n";
            }

            cout << flush;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}


//=====================
// Головна програма
//=====================
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    vector<int> sizes = { 1000, 10000, 100000 };
    vector<string> types = { "Growth", "Random", "Decrease" };

    vector<Algo> algos = {
        { "Bubble Basic",               runBubbleSort },
        { "Bubble Flag",                runBubbleSortFlag },
        { "Bubble LastSwap",            runBubbleSortLastSwap },
        { "Shaker",                     runShakerSort },
        { "Insertion",                  runInsertionSort },
        { "Selection",                  runSelectionSort },
        { "Merge",                      runMergeSort },
        { "Counting",                   runCountingSort },
        { "Radix",                      runRadixSort },
        { "Bucket",                     runBucketSort },
        { "Heap",                       runHeapSort },
        { "Quick",                      runQuickSort }
    };

    // Підготовка progress-bar
    progressBars.resize(algos.size());
    for (int i = 0; i < algos.size(); i++) {
        progressBars[i].name = algos[i].name;
        progressBars[i].percent = 0;
    }

    vector<thread> workers;

    // Стартуємо дисплей потік
    thread displayThread(progressDisplayThread);
    vector<vector<vector<Stats>>> allResults(
        algos.size(),
        vector<vector<Stats>>(sizes.size(),
            vector<Stats>(types.size()))
    );
    // Запуск алгоритмів у потоках
    for (int index = 0; index < algos.size(); index++)
    {
        workers.emplace_back([&, index]()
            {
                auto algo = algos[index];
                int step = 0;

                for (int i = 0; i < sizes.size(); i++)
                {
                    int n = sizes[i];

                    allResults[index][i][0] = algo.run(generateAscending(n));
                    progressBars[index].percent = ++step * 100 / (sizes.size() * 3);

                    allResults[index][i][1] = algo.run(generateRandom(n));
                    progressBars[index].percent = ++step * 100 / (sizes.size() * 3);

                    allResults[index][i][2] = algo.run(generateDescending(n));
                    progressBars[index].percent = ++step * 100 / (sizes.size() * 3);
                }

                progressBars[index].percent = 100;
            });
    }

    for (auto& t : workers)
        t.join();

    progressRunning = false;
    displayThread.join();

    // --- тепер друкуємо результати усіх алгоритмів ---
    for (int i = 0; i < algos.size(); i++)
    {
        printTable(sizes, types, allResults[i], algos[i].name);
    }

    return 0;
}

