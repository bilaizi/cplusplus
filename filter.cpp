#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

auto main() -> int {
        cout << "[filter.cpp]" << endl;
        // Initializing a vector containing integer elements
        vector<int> numbers;
        for (int i { 0 }; i < 20; ++i)
                numbers.push_back(i);
        // Displaying the elements of numbers
        cout << "The original numbers: " << endl;
        copy(begin(numbers), end(numbers), ostream_iterator<int>(cout, " "));
        cout << endl;
        // Declaring a vector containing int elements
        vector<int> primes;
        // Filtering the vector
        copy_if(begin(numbers), end(numbers), back_inserter(primes), 
                [](int n) {
                        if(n < 2)
                                return false;
                        else {
                                for (int j = 2; j < n; ++j) {
                                        if (n % j == 0)
                                                return false;
                                }
                                return true;
                        }
                }
        );
        // Displaying the elements of primes using copy() function
        cout << "The primes numbers: " << endl;
        copy(begin(primes), end(primes), ostream_iterator<int>(cout, " "));
        cout << endl;
        // Declaring a vector containing int elements
        vector<int> nonPrimes;
        // Filtering the vector
        remove_copy_if(begin(numbers), end(numbers), back_inserter(nonPrimes), 
                [](int n) {
                        if(n < 2)
                                return false;
                        else {
                                for (int j = 2; j < n; ++j) {
                                        if (n % j == 0)
                                                return false;
                                }
                                return true;
                        }
                }
        );
        // Displaying the elements of nonPrimes using copy() function
        cout << "The non-primes numbers: " << endl;
        copy(begin(nonPrimes), end(nonPrimes), ostream_iterator<int>(cout, " "));
        cout << endl;
        return 0;
}
