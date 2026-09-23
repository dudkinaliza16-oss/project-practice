#include <iostream>

int main() {
    int a = 0;
    int b = 0;
    int c = 0;
    std::cout << "Write three numbers: ";
    if (std::cin >> a >> b >> c) {
        int sum = a + b + c;
        std::cout << "Sum: " << sum << std::endl;
    } else {
        std::cout << "Not a number!" << std::endl;
    }
    return 0;
}