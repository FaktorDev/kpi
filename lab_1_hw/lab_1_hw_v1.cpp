#include <math.h>
#include <stdio.h>

int main2()
{
    const double PI = 3.14159265358979323846;
    double result_a, result_b, result_c;
    double a, b, c;

    printf("Enter side a: ");
    scanf_s("%lf", &a);
    printf("Enter side b: ");
    scanf_s("%lf", &b);
    printf("Enter side c: ");
    scanf_s("%lf", &c);

    result_a = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
    result_b = acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * a * c));
    result_c = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));

    printf("Result A: %.2f\n", result_a * (180.0 / PI));
    printf("Result B: %.2f\n", result_b * (180.0 / PI));
    printf("Result C: %.2f\n", result_c * (180.0 / PI));

    return 0;
}
