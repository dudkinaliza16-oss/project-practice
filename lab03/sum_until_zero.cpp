#include <iostream>
#include <vector>
using namespace std;

void printAsterisk(const int countAsterisk) {
    for (int i = 0; i < countAsterisk; i++) {
        cout << "*";
    }
    cout << endl;
}

int main() {
    int t;
    vector<int> listOfNumbers;
    int sum = 0;

    while (cin >> t && t != 0) {
        sum += t;
        listOfNumbers.push_back(t);
    }

    if (listOfNumbers.empty()) {
        cout << "Error: Empty list of numbers" << endl;
        return 1;
    }

    cout << "Sum: " << sum << endl;
    cout << "Count: " << listOfNumbers.size() << endl;

    for (int i = 0; i < listOfNumbers.size(); i++) {
        printAsterisk(listOfNumbers[i]);
    }

    return 0;
}
