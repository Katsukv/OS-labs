#include <iostream>
#include <sstream>

using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        istringstream iss(line);
        int num;
        while (iss >> num) {
            cout << num * num * num << " ";
        }
        cout << endl;
    }
    return 0;
}