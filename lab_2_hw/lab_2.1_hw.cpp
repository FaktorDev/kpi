#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const string F_ch = "ch(x)";
const string F_fabs_x_plus_1 = "1+|x|";
const string F_pow_from_1_plus_pow_x_2 = "(1+x^2)^x";

double ch(double x)
{
    return cosh(x);
}

double fabs_x_plus_1(double x)
{
    return fabs(x) + 1;
}

double pow_from_1_plus_pow_x_2(double x)
{
    return pow((1 + pow(x, 2)), x);
}

void ifelse_variant()
{
    double x = 0;
    cout << "Input x:";
    cin >> x;

	double f_ch = ch(x);
	double f_fabs_x_plus_1 = fabs_x_plus_1(x);
	double f_pow_from_1_plus_pow_x_2 = pow_from_1_plus_pow_x_2(x);

    if (ch(x) <= fabs_x_plus_1(x) && ch(x) <= pow_from_1_plus_pow_x_2(x))
    {
        cout << F_ch << " = " << f_ch << endl;
        if (fabs_x_plus_1(x) <= pow_from_1_plus_pow_x_2(x))
        {
            cout << F_fabs_x_plus_1 << " = " << f_fabs_x_plus_1 << endl;
            cout << F_pow_from_1_plus_pow_x_2 << " = " << f_pow_from_1_plus_pow_x_2 << endl;
        }
        else
        {
            cout << F_pow_from_1_plus_pow_x_2 << " = " << f_pow_from_1_plus_pow_x_2 << endl;
            cout << F_fabs_x_plus_1 << " = " << f_fabs_x_plus_1 << endl;
        }
    }
    else if (fabs_x_plus_1(x) <= ch(x) && fabs_x_plus_1(x) <= pow_from_1_plus_pow_x_2(x))
    {
        cout << F_fabs_x_plus_1 << " = " << f_fabs_x_plus_1 << endl;
        if (ch(x) <= pow_from_1_plus_pow_x_2(x))
        {
            cout << F_ch << " = " << f_ch << endl;
            cout << F_pow_from_1_plus_pow_x_2 << " = " << f_pow_from_1_plus_pow_x_2 << endl;
        }
        else
        {
            cout << F_pow_from_1_plus_pow_x_2 << " = " << f_pow_from_1_plus_pow_x_2 << endl;
            cout << F_ch << " = " << f_ch << endl;
        }
    }
    else
    {
        cout << F_pow_from_1_plus_pow_x_2 << " = " << f_pow_from_1_plus_pow_x_2 << endl;
        if (ch(x) <= fabs_x_plus_1(x))
        {
            cout << F_ch << " = " << f_ch << endl;
            cout << F_fabs_x_plus_1 << " = " << f_fabs_x_plus_1 << endl;
        }
        else
        {
			cout << F_fabs_x_plus_1 << " = " << f_fabs_x_plus_1 << endl;
            cout << F_ch << " = " << f_ch << endl;
        }
    }
    char pause;
    cin >> pause;
	system("cls");
}

void dict_variant()
{
    double x = 0;
    cout << "Input x:";
    cin >> x;
    map<string, double> dict;
    dict[F_ch] = ch(x);
    dict[F_fabs_x_plus_1] = fabs_x_plus_1(x);
    dict[F_pow_from_1_plus_pow_x_2] = pow_from_1_plus_pow_x_2(x);

    vector <pair<string, double>> vec(dict.begin(), dict.end());

    std::sort(vec.begin(), vec.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    for (auto& [key, value] : vec) {
        std::cout << key << " => " << value << std::endl;
    }
    char pause;
    cin >> pause;
    system("cls");
}

int main()
{
    while (true)
    {
		cout << "Choose variant (1 - ifelse, 2 - dict, 0 - exit): ";
		int variant;
		cin >> variant;
        switch (variant)
        {
        case 0:
            return 0;
        case 1:
            ifelse_variant();
            break;
        case 2:
            dict_variant();
            break;
        default:
            break;
        }
    }
}

