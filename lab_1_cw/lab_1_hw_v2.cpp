#include <math.h>
#include <cmath>
#include <iostream>

using namespace std;

void main()
{
	const double PI = 3.14159265358979323846;
	double result_a, result_b, result_c, a, b, c;

	cout << "Enter side a: ";
	cin >> a;
	cout << "Enter side b: ";
	cin >> b;
	cout << "Enter side c: ";
	cin >> c;

	result_a = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
	result_b = acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * a * c));
	result_c = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));

	cout << "Result A: " << result_a * (180 / PI) << endl;
	cout << "Result B: " << result_b * (180 / PI) << endl;
	cout << "Result C: " << result_c * (180 / PI) << endl;
}