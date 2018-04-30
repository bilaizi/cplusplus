#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// Function for logging the flow
int addition(const int& x, const int& y) {
        cout << x << " + " << y << endl;
        return x + y;
}

auto main() -> int {
        cout << "[fold.cpp]" << endl;
        // Initializing a vector containing integer elements
        vector<int> numbers = {0, 1, 2, 3, 4};
        // Calculating the sum of the value in the vector
        auto foldl1 = accumulate(begin(numbers), end(numbers), 0,  std::plus<int>());
        // Calculating the sum of the value in the vector
        auto foldr1 = accumulate(rbegin(numbers), rend(numbers), 0, std::plus<int>());
        // Displaying the calculating result
        cout << "foldl1 result = " << foldl1 << endl;
        cout << "foldr1 result = " << foldr1 << endl;
        // Calculating the sum of the value in the vector from left to right
        cout << "foldl2" << endl;
        auto foldl2 = accumulate(begin(numbers), end(numbers), 0, addition);
        // Calculating the sum of the value in the vector from right to left
        cout << endl << "foldr2" << endl;
        auto foldr2 = accumulate(rbegin(numbers), rend(numbers), 0, addition);
        cout << endl;
        // Displaying the calculating result
        cout << "foldl2 result = " << foldl2 << endl;
        cout << "foldr2 result = " << foldr2 << endl;
        return 0;
}
