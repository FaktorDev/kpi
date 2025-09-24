#include <math.h>
#include <iostream>

using namespace std;

void main() 
{
	const double PI = 3.14159265358979323846;	
	double result_a,result_b,x,y,z;

	cout << "Enter x: ";
	cin >> x;
	cout << "Enter y: ";
	cin >> y;
	cout << "Enter z: ";
	cin >> z;
	result_a = (2*cos(x-PI/6)/(1/(2+pow(sin(y),2))));
	result_b = 1+(pow(z,2)/(3+pow(x,2)/5));
	cout << "Result A: " << result_a << endl;
	cout << "Result B: " << result_b << endl;
}