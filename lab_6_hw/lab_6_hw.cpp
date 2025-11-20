#include <iostream>
#include <string>
using namespace std;

int main() {
    string text, from, to;

    cout << "Enter the phrase:\n";
    getline(cin, text);

    cout << "Enter the word you want to replace:\n";
    cin >> from;

    cout << "Enter the word to replace:\n";
    cin >> to;

    int pos = 0;

    while (true) {
        pos = text.find(from, pos);

        if (pos == -1)
            break;

        text.erase(pos, from.length());
        text.insert(pos, to);

        pos += to.length();
    }

    cout << "Result:\n" << text << endl;

    return 0;
}
