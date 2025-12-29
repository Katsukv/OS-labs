#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    string line;
    int sum = 0;
    while (getline(cin, line)) {
        istringstream iss(line);
        int num;
        while (iss >> num) {
            sum += num;
        }
    }
    cout << "Total sum: " << sum << endl;
    return 0;
}