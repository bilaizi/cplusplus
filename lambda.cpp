#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

auto main() -> int {
    vector<int> vect;
    for (int i = 0; i < 10; ++i)
        vect.push_back(i);
    cout << "Original Data:" << endl;
    for_each(begin(vect), end(vect), [](int n){ cout << n << " "; });
    cout << endl;
    for_each(
             begin(vect),
             end(vect),
             [](auto n) {
                cout << n << " is";
                if(n < 2) {
                    cout << " not";
                } else {
                    for (int j = 2; j < n; ++j) {
                        if (n % j == 0){
                            cout << " not";
                            break;
                        }
                    }
                }
                cout << " prime number" << endl;
             }
    );
    cout << endl;
    for_each(begin(vect), end(vect), [](int& x) { x *= 2; });
    cout << "Squared Data:" << endl;
    for_each(begin(vect), end(vect), [](int n) { cout << n << " "; });
    cout << endl << endl;
    return 0;
}
