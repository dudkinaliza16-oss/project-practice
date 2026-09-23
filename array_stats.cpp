#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cout << "Enter count of numbers: ";
    if (!(std::cin >> n) || n <= 0) {
        return 1;
    }

    std::vector<int> numbers;

    std::cout << "Enter " << n << " numbers: ";
    for (int i = 0; i < n; ++i) {
        int temp = 0;
        std::cin >> temp;
        numbers.push_back(temp);
    }

    int sum = 0;
    int min = numbers[0];
    int max = numbers[0];
    float arithmeticMean = 0.0f;

    std::cout << "You entered: ";
    for (int i = 0; i < n; ++i) {
        sum += numbers[i];
        if (numbers[i] > max) {
            max = numbers[i];
        }
        if (numbers[i] < min) {
            min = numbers[i];
        }
    }
    arithmeticMean = static_cast<float>(sum) / n;

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Min: " << min << std::endl;
    std::cout << "Max: " << max << std::endl;
    std::cout << "Mean: " << arithmeticMean << std::endl;

    return 0;
}