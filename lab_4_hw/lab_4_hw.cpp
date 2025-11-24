#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    int n, m;
    cout << "Enter where we find (size A): ";
    cin >> n;
    cout << "Enter subsequence m (size B, m < n): ";
    cin >> m;

    // Allocating memory for arrays
    int* A = new int[n];
    int* B = new int[m];

    // A
    for (int i = 0; i < n; i++)
        A[i] = rand() % 10;

    // B
    for (int i = 0; i < m; i++)
        B[i] = rand() % 10;

    // Conclusion
    cout << "A:\n";
    for (int i = 0; i < n; i++)
        cout << A[i] << " ";
    cout << endl;

    cout << "B:\n";
    for (int i = 0; i < m; i++)
        cout << B[i] << " ";
    cout << endl;

    // Finding the occurrence of B in A
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
        cout << "Subsequence B not found in A" << endl;

        delete[] A;
        delete[] B;
        return 0;
    }

    // Delete a subsequence(manual element shifting)
    for (int i = start; i < n - m; i++) {
        A[i] = A[i + m];
    }

    // new array size after deletion
    int newSize = n - m;

    cout << "A after removing subsequence B: \n";
    for (int i = 0; i < newSize; i++)
        cout << A[i] << " ";
    cout << endl;

    // Freeing memory
    delete[] A;
    delete[] B;

    return 0;
}
