#include <iostream>
#include <cmath>

int main() {
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    std::cout << "Enter coefficients a and b for 'ax^2 + bx + c = 0': ";

    if (!(std::cin >> a >> b >> c)) {
        std::cout << "Incorrect input! " << std::endl;
        return 1;
    }

    float D = b * b - (4 * a * c);


    if (a != 0.0f) {
        if (D > 0.0f) {
            float x1 = (-b + std::sqrt(D)) / (2 * a);
            float x2 = (-b - std::sqrt(D)) / (2 * a);
            std::cout << "Two real roots: " << x1 << ", " << x2 << std::endl;
        } else if (D == 0.0f) {
            float x = -b / (2 * a);
            std::cout << "One real root: " << x << std::endl;
        } else {
            std::cout << "There are no real roots0 0 5! " << std::endl;
        }
    } else {
        std::cout << "Error: coefficient 'a' cannot be zero." << std::endl;
    }
    return 0;
}
