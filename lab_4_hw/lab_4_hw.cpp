#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the order of the matrix n: ";
    cin >> n;

    // Dynamic memory allocation for matrix
    double** A = new double* [n];
    double** B = new double* [n];
    double** C = new double* [n];

    for (int i = 0; i < n; ++i) {
        A[i] = new double[n];
        B[i] = new double[n];
        C[i] = new double[n];
    }

    cout << "Enter the elements of the first matrix A:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    cout << "Enter the elements of the second matrix B:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> B[i][j];

    for (int i = 0; i < n; ++i) {
        double product = 1;
        for (int j = 0; j < n; ++j)
            product *= B[i][j];

        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] + product;
    }

    cout << "\nThe resulting matrix C:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << C[i][j] << " ";
        cout << endl;
    }

    for (int i = 0; i < n; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}