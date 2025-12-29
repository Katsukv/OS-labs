#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    const int N = 5;
    string line;
    while (getline(cin, line)) {
        istringstream iss(line);
        int num;
        while (iss >> num) {
            cout << num + N << " ";
        }
        cout << endl;
    }
    return 0;
}