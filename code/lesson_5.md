# introduction 
For this lesson, i want to expand namespace aml(Advanced metaprogramming library) and defining some new things :
  1. aml::type_element
  2. aml::none_type
  3. aml::type_list::at<>()

# aml::none_type
``` C++
struct none_type {};
```
It is a special type in order to represent type void or none

# aml::type_element
At first, i tried to write a type iterator that is able to iterate throughout the type_list. But after many hours, I 
noticed that it is impossible to write something like that in statically typed language like c++. Let me explain:
First thing I decided about this struct was its syntax. And this was going to be somthing like:
``` C++
// pseudo code
aml::type_iterator<2,decltype(list1)> iter; // as you can see first template arg is index and second is list1's type
```
When i want to increase this iterator or get the next type in list or in other words when i want to iterate i have to do:
``` C++
iter++; // if it is iterator, i have to be able to call this
```
But let's see what happens. When i call ```iter++;``` it is going to increase index by one and for example if iter 
is defined as ```aml::type_iterator<1,decltype(some_list)> iter```, the result of operation++ will be ```aml::type_iterator<2,decltype(some_list)>```
. And assigning this type to iter is impossible because types are different. When look first, it seems not true because 
you know both of them are ```aml::type_iterator```. But in c++ sometimes only class/struct name is not enough to identify object's type.
It likes comparing std::vector<int> and std::vector<std::string> and saying that they are same. You see? They are different types and 
objects of this types can't be assigned. This is why creating an type iterator like ```aml::type_iterator<index, list_type>``` is impossible in a statically 
typed language like C++. But don't be desperate. There is always solution and the solution is at metafunction. But before it let me introduce you '
something new: aml::type_element. Here is implementation:
``` C++
  template<typename T = none_type>
	struct type_element {
		using type = T;
	};
```
This struct stores type and when you want to get reach to the type use:
``` C++
type_element<int> b;
b::type i = 12; // use when you want to store type of an an object
```
Since we couldn't iterate throughout our type_lists, I thought it will be pointless to name this class as type_iterator.
Maybe we can't iterate in c++ in this type lists but at least we can write a function that returns type_element object from
particular list. Here is ```at``` member function of struct type_list:
``` C++
    // inside struct type_list
    template<std::size_t index>
		constexpr decltype(auto) at() {
			static_assert(index >= 0 && index < sizeof...(V), "Index out of range!");
			return aml::type_element<std::tuple_element_t<index,std::tuple<V...>>>();
		}
```
I didn't design this like we can call ```ls.at(2)``` because, in this form, ```static_assert``` will fail because in c++ 
function argument passed can't be a ```constexpr``` and ```static_assert``` works only if the argument is constexpr. 
So I used template ```std::size_t index```. because, every template argument must be either a type or a constexpr value.
As you can see, static assert uses ```sizeof...(V)``` and in here, V is our variadic template argument(i.e. all types in V variadic type template):
``` C++
template<typename ...V>
struct type_list;
```
sizeof...() is special operator that returns the size of variadic params. After static_assert reassuming compiler that 
index is not out of range we return ```aml::type_element<std::tuple_element_t<index,std::tuple<V...>>>()```. std::tuple_element_t is a useful 
STL template struct feature: ``` using tuple_element_t<I,std::tuple<Head,Tail...>> = std::tuple_element<I,std::tuple<Head,Tail...>>::type;```.
Let's see usages:
``` C++
aml::type_list<int,int,char,std::string> ls;
auto i = ls.at<1>(); // type of i ----> int
auto z = ls.at<3>(); // type of z ----> std::string
auto d = ls.at<5>(); // ERROR! STATIC_ASSERT FAILED!
```
# Note:

Don't try this, because it won't work:
``` C++
aml::type_list<int,std::string> ls;
for (int i = 0; i < 2; i++)
{
  auto s = ls.at<i>();
}
```
Becuse i is not a constexpr and template argument must be either a type or a constexpr value.

Or don't try:
``` C++
	aml::type_list<int,float,std::string,long,char> ls1;
	constexpr int arr[2] = {0,1};
	for (auto& i: arr)
	{
		auto z = ls1.at<i>();
	}
```

Because they will fail. I said, type iteration is too hard. So here is the idea:
if you want to write type iterator, you can do it by polimorphism. I.e. write a virtual base class that holds 
list's type and after this, define a derived class that holds index. so start by pointer to base and assign ```new Iterator<index,types>();```
and after that write ```next(Iterator&)``` and delete it. and assign it ```new Iterator<index+1,same_type>()```. As 
pseudo-code:
``` C++
void next(Base* iter)
{
  delete iter;
  iter = new ...; // writing implementation is the topic of another lesson
}
```




