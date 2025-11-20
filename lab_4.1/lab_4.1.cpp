#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    int n;
    cout << "Enter n (matrix size): ";
    cin >> n;

    double** A = new double* [n];
    double** B = new double* [n];
    double** C = new double* [n];

    for (int i = 0; i < n; i++) {
        A[i] = new double[n];
        B[i] = new double[n];
        C[i] = new double[n];
    }

    // A
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10 - 5; // -5..4

    // B
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            B[i][j] = rand() % 10 - 5;

    cout << "\nMatrix A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << A[i][j] << " ";
        cout << "\n";
    }

    // B
    cout << "\nMatrix B:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << B[i][j] << " ";
        cout << "\n";
    }

	// C caltculation
    for (int i = 0; i < n; i++) {
        double prod = 1.0; 

        for (int j = 0; j < n; j++)
            prod *= B[i][j];

        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + prod;
    }

    // C
    cout << "\nMatrix C:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << C[i][j] << " ";
        cout << "\n";
    }

    // Звільнення памʼяті
    for (int i = 0; i < n; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
