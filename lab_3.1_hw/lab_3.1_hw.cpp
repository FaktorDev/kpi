#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    cout << "Input size of table A x B:\n";
	cout << "A = ";
	double a;
	cin >> a;
	cout << "B = ";
	double b;
	cin >> b;
	cout << "Input size of step:\n";
	double step;
	cin >> step;
	system("cls");

	cout << "\t";
	for (double i = a; i <= b; i += step)
	{
		cout << i << "|" << "\t";
	}
	cout << "\n";
	for (double i = a; i < b; i += step)
	{
		cout << "---------";
	}
	cout << "\n";

	for (double i = a; i <= b; i += step)
	{
		for(double j = a; j <= b; j += step)
		{
			if (j == a)
			{ 
				cout << i <<"  |" << "\t";
			}
			
			double res = i * j;
			cout << res << "\t";
		}
		cout << "\n";
	}
}

