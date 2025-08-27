#include <iostream>
#include <stdexcept>
#include <type_traits>

template<typename T>
void print_var(T a)
{
	if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value)
		std::cout << std::boolalpha << a << '\n';
	else if constexpr (std::is_same<std::string,decltype(a)>::value || std::is_same<const char*,decltype(a)>::value)
		std::cout << a << '\n';
	else 
		throw std::runtime_error("Error, neither a string nor an integral!\n");
}

int main()
{
	print_var("Hello!");
	print_var(12);
	print_var(12.34);
	print_var(true);
	return 0;
}
