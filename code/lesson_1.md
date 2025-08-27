# First 
This is the first lesson. See the file lesson_1.cpp. 
# Second thing 
``` C++
#include <iostream>
#include <type_traits>
#include <stdexcept>
```
iostream guy is essential for std::cout, type_traits guy is essential for std::is_same, std::is_floating_point, std::is_integral and so on. And stdexcept is required for 
throwing std::runtime_error. What is a template? So sometimes, we have to write a function or something like user defined types, that works for several types.
At this point, templates come with a solution. Template is basically something that you can use it in order to write a generalized function (or class, struct. for this lesson we are going to work with template function)
that works for different types( with support of concepts even user defined types
work correctly in metaprogramming). So let's assume we want to write a function that prints printable things like int, string, bool, char, double or float(in this particular code
i defined only these types "printable"). It is very tedious writing something like that: 
``` C++
void print(int v)
{
  // some code 
}
void print(std::string b)
{
  // some code
}
void print(float f)
{
  // some code
}
```
Instead of doing this, we can simply write a pseudo code: 
``` C++
// template magic
void print() // magic here 
{
  // works for all 
}
```
### Step 1: 
``` C++
template<typename T>
void print_var(T a)
```
It is template definition for funtion. In there typename means T is a type object(int, char, bool, std::string and so on) and as parameter we take variable a which is T type object.

### Step 2:
``` C++
{
	if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value)
		std::cout << std::boolalpha << a << '\n';
	else if constexpr (std::is_same<std::string,decltype(a)>::value || std::is_same<const char*,decltype(a)>::value)
		std::cout << a << '\n';
	else 
		throw std::runtime_error("Error, neither a string nor an integral!\n");
}
```
In there, ``` if constexpr ``` structure forces compiler to detect errors in function calls in compile-time. It means when user try to write print_var(std::thread(some_function)) since 
std::thread object is not a printable object, it fails in subsitution(i.e. std::same_as, std::is_integral and std::is_floating_point all fail). In such cases ``` if constexpr ``` helps 
to detect these errors in compile-time. In code above, std::is_integral, std::is_same  and std::is_floating_point are defined in type_traits. decltype(a) just returns type of the object
and is built-in c++ functionality. So take a look if you don't understand:
``` C++
// example code
int a = 12;
decltype(a) b = a; // b is int
int* c = new int(89);
decltype(c) d; // d is int*
delete c;
```
### Step 3:

Finally, when you try to call print_var(12), print_var("Hello!"), all will work but when you try to write print_var(std::vector<int>{1,2,3}) it will fail. 
