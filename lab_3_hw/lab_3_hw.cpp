#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    double x, eps;
    cout << "Enter x (|x| < 1): ";
    cin >> x;
    cout << "Enter epsilon (0 < eps < 1): ";
    cin >> eps;

    double term = x;    // перший член ряду
    double sum = term;   
    int n = 1;

    while (fabs(term) > eps)
    {
        n++;
        term = pow(-1, n - 1) * pow(x, n) / n;
        sum += term;
    }

    cout << "\nCalculated ln(1 + x) ≈ " << sum << endl;
    cout << "Check with log(1 + x) = " << log(1 + x) << endl;
    cout << "Number of terms used: " << n << endl;

    return 0;
}
