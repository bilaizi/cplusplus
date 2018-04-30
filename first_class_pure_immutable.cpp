#include <functional>
#include <iostream>

using namespace std;

class Value {
public:
        const int value;
        Value(int v) : value{ v }{}
};

class Function {
public:
        const int x, y;
        Function(int _x, int _y) : x{ _x }, y{ _y }{}
        Value addition() const {
                return Value{ x + y };
        }

        Value subtraction() const {
                return Value{ x - y };
        }

        Value multiplication() const {
                return Value{ x * y };
        }

        Value division() const {
                return Value{ x / y };
        }
};

auto main() -> int {
        cout << "[first_class_pure_immutable.cpp]" << endl;
        int i, a, b;
        cout << "Select mode:" << endl;
        cout << "1. Addition" << endl;
        cout << "2. Subtraction" << endl;
        cout << "3. Multiplication" << endl;
        cout << "4. Division" << endl;
        cout << "Choice: ";
        cin >> i;
        if(i < 1 || i > 4) {
                cout << "Please select available mode!";
                return 1;
        }
        cout << "a -> ";
        cin >> a;
        while (cin.fail()) {
                cin.clear();
                cin.ignore(INT8_MAX, '\n');
                cout << "You can only enter numbers.\n";
                cout << "Enter a number for variable a -> ";
                cin >> a;
        }
        cout << "b -> ";
        cin >> b;
        while (cin.fail()) {
                cin.clear();
                cin.ignore(INT8_MAX, '\n');
                cout << "You can only enter numbers.\n";
                cout << "Enter a number for variable b -> ";
                cin >> b;
        }
        Function func(a, b);
        auto callableAdd = mem_fn(&Function::addition);
        auto callableSub = mem_fn(&Function::subtraction);
        auto callableMul = mem_fn(&Function::multiplication);
        auto callableDiv = mem_fn(&Function::division);
        auto valueAdd = callableAdd(func);
        auto valueSub = callableSub(func);
        auto valueMul = callableMul(func);
        auto valueDiv = callableDiv(func);
        cout << "The result" << endl;
        cout << "addition = " << valueAdd.value << endl;
        cout << "subtraction = " << valueSub.value << endl;
        cout << "multiplication = " << valueMul.value << endl;
        cout << "division = " << valueDiv.value << endl;
        return 0;
}
