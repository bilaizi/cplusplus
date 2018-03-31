#include <algorithm>
#include <iostream>
#include <vector>

void displayAsInstruction(int number){
    if (number > 0)
        std::cout << '+' << number << '\n';
    else if (number == 0)
        std::cout << "nop" << '\n';
    else if (number < 0)
        std::cout << number << '\n';
}

template<typename Range, typename Function>
Function for_each(Range& range, Function f) {
    return std::for_each(std::begin(range), std::end(range), f);
}

auto main()->int{
    std::vector<int> numbers = {1, -2, 3, 0, 5};
    for (auto number : numbers)
        displayAsInstruction(number);
    std::cout << std::endl;  
    for_each(numbers, displayAsInstruction);
    return 0;
}
