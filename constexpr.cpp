constexpr int factorial(int n) {
	return n <= 1 ? 1 : (n * factorial(n - 1));
}
constexpr int factorial(int n) {
int result = n;
while(n > 1)
result *= --n;
return result;
}
