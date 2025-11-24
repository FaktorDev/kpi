#include <iostream>
#include <cmath>
using namespace std;

// f(x) = x^5 - x - 0.2
double func(double x) {
    return pow(x, 5) - x - 0.2;
}

// Half-division method
double bisection(double (*f)(double), double a, double b, double eps) {
    if (f(a) * f(b) > 0) {
        cout << "There is no guarantee of a root on the interval (f(a)*f(b) > 0)." << endl;
        return NAN;
    }

    double c;

    while ((b - a) > eps) {
        c = (a + b) / 2.0;

        if (abs(f(c)) < eps)
            break;

        if (f(a) * f(c) < 0)
            b = c;
        else
            a = c;
    }

    return (a + b) / 2.0;
}

int main() {
    double a = 1.0;
    double b = 1.1;
    double eps = 0.00001;


    double root = bisection(func, a, b, eps);

    if (!isnan(root)) {
        cout << "Approximate root: " << root << endl;
        cout << "Check f(root) = " << func(root) << endl;
    }

    return 0;
}
