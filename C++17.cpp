#include <array>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>
 
auto main() -> int {
	std::array<char, 10> str{ "42" };
	int result;
	std::from_chars(str.data(), str.data()+str.size(), result);
	std::cout << result;
	if(auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), result); ec == std::errc{})
		std::cout << std::string_view(str.data(), ptr - str.data());
}
