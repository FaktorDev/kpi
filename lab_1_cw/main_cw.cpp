#include <math.h>
#include <iostream>

using namespace std;

void main()
{
	double result, x, y;

	cout << "Enter x: ";
	cin >> x;
	cout << "Enter y: ";
	cin >> y;

	result = sqrt(fabs(pow(x,2)+5*pow(y,2)+0.12))+37.5;
	cout << "Result: " << result << endl;
}