#include <array>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>
 
auto main() -> int {
	std::array<char, 10> str{ "42" };
	int result;
	std::from_chars(str.data(), str.data()+str.size(), result);
	std::cout << result << std::endl;
	if(auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), result); ec == std::errc{})
		std::cout << std::string_view(str.data(), ptr - str.data()) << std::endl;
	return 0;
}
/*
42 
42
*/

#include <string>
#include <string_view>
#include <iostream>

using namespace std;

string_view extractExtension(string_view fileName){
	return fileName.substr(fileName.rfind('.'));
}

auto main() -> int {
	// C++ std::string.
	string fileName = R"(c:\temp\my file.ext)";
	cout << "C++ string: " << extractExtension(fileName) << endl;
	// C-style string.
	const char* cString = R"(c:\temp\my file.ext)";
	cout << "C string: " << extractExtension(cString) << endl;
	// String literal.
	cout << "Literal: " << extractExtension(R"(c:\temp\my file.ext)") << endl;
	// Raw string buffer with given length.
	const char* raw = "test.ext";
	size_t length = 8;
	cout << "Raw: " << extractExtension(string_view(raw, length)) << endl;
	// Assigning the result to a std::string.
	string extension = extractExtension(fileName).data();
	return 0;
} 
/*
C++ string: .ext
C string: .ext
Literal: .ext
Raw: .ext
*/
