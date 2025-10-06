#include <iostream>
using namespace std;

int main() {
    int y;
    cout << "Input year (y > 0): ";
    cin >> y;

    if (y <= 0) {
        cout << "Year can be only over then 0" << endl;
        return 0;
    }

    int c = (y - 1) / 100 + 1;

    switch (c) {
    case 20:
        cout << "Year " << y << " belongs to the XX century." << endl;
        break;
    case 21:
        cout << "Year " << y << " belongs to the XXI century." << endl;
        break;
    default:
        cout << "Year " << y << " belongs to the " << c << "th century." << endl;
        break;
    }

    return 0;
}
