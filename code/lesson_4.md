# Introduction 
For this lesson, I want to write a small template metaprogramming library that is useful for projects. 
I decided to name namespace ```aml``` which stands for Advanced Metaprogramming Library.
Before dive into the code I want to introduce you our library's some functions and structures:
  1. aml::type_list - struct for storing types as a list elements
  2. aml::_if - template struct for making type decisions in compile time based on constexpr conditions' results
  3. aml::empty - returns if a type_list is empty or not
  4. aml::front - returns first type in the type list(use result member )
  5. aml::end - returns last type in the type list(use result member )
  6. aml::pop_front - returns type list without first type
  7. aml::reverse - returns reverse of input type list
  8. aml::merge - merges two type list and returns new one

# Step 1:
We start with writing _if metastructure:
``` C++
    template<bool condition,typename THEN, typename ELSE>
	struct _if;

	template<typename THEN, typename ELSE>
	struct _if<true,THEN,ELSE> {
		using type = THEN;
	};

	template<typename THEN, typename ELSE>
	struct _if<false,THEN,ELSE> {
		using type = ELSE;
	};
```
As you can see I wrote two specialization for this struct. One for the case when the condition is ```true``` and one for 
when the condition is ```false```. If it is true, the type will be THEN, and if it is false type will be false.

# Step 2:
Let's write type_list structure:

``` C++
    template<typename ...V>
	struct type_list{
		constexpr decltype(auto) operator=(const type_list<V...>& other){
			*this = other;
			return *this;
		}
	};
```

I wrote only operator= becuse it will be the most important throughout our work. You can write operator+ and operator+= yourself 
and use them for merging two ```type_list``` variables together. I don't wrote them because there is already a function named merge for this 
purpose. Let's see usage of this struct:
``` C++
aml::type_list<int,int> list1; // correct usage
aml::type_list<float,float,user_defined_type,std::string> list2; // also OK
aml::type_list ls = {int, char}; // WRONG! This kind of structures are not suitable for c++ syntax and language!
```

# Step 3:
Let's write some metafunctions that returns type_lists and types
1. empty
``` C++
	template<typename ...P>
	constexpr bool empty(const type_list<P...>& d) {
		static constexpr type_list<> a;
		return (typeid(d) == typeid(a));
	}
```
Returns if the type list is empty or not. The only thing this function does is to compare input type list with an empty 
type list. In such cases typeid() operator can be used which returns std::type_info object. You can get the name of this 
type with typeid(object).name(). But this member function of std::type_info returns a mangled name which is not human readable(for 
example for type_list object , 334type_listIFRG342 or something like this). In order to make it readable you can use 
abi::__cxa_demangle function(for this function, include cxxabi.h in your file). If you want to get more information about typeid() operator and typeid().name(), 
visit https://en.cppreference.com/w/cpp/language/typeid.html adress.

2.front 
``` C++
	template<typename First,typename ...Types>
	struct front;

	template<typename First,typename ...Types>
	struct front<type_list<First,Types...>> {
		using result = First;
	};
```
usage of ```aml::front```:
``` C++
using namespace aml; 
type_list<int,float,char> ls;
front<decltype(ls)>::result a = 12; // type of a is int
type_list<std::string,std::ostream> list2;
front<decltype(list2)>::result b = "Hello!"; // type of b is std::string
```
3. merge
``` C++
	template <typename ...V,typename ...D>
	constexpr decltype(auto) merge(const type_list<V...>& first, const type_list<D...>& second)
	{
		return type_list<V...,D...>();
	}
```
As you can notice the only thing this function does is taking variadic template parameters and use them in merging.
5. reverse 
writing a function that reverses a type list is also important:
``` C++
	template<typename First, typename ...Var>
	constexpr decltype(auto) reverse(type_list<First,Var...> ls)
	{ 
		return merge(reverse(type_list<Var...>()),type_list<First>());	
	}

	constexpr type_list<> reverse(type_list<> ls) {
		return type_list<>();	
	}
```
This function works recursively as you can see, and it is also very important to overload this function for empty type list. 
Otherwise code finally runs: 
``` return merge(reverse(type_list<>()),type_list<Last_type>()); ``` and since in this case we pass template to the 
function like:
``` C++
template<typename First = Nothning, typename ...Var = Nothing >
```
Compiler detect error because in C++ variadic template typenames can be nothing but typename itself can't be. For this reason define 
overloaded reverse.

6. end
``` C++
	template<typename First, typename ...Rest>
	struct end;

	template<typename First, typename ...Rest>
	struct end<type_list<First,Rest...>> {
		using result = front<decltype(reverse(type_list<First,Rest...>()))>::result;
	};
```
And our final metastructure is ```aml::end```. Logically, the type in the end of a type list is actually the first type in 
reverse of this list. It means, we need to get the reverse and to get the first type in reverse type_list. Usage of ```end```:
``` C++
using namespace std;
type_list<int,int,float> list1;
end<decltype(list1)>::result i = 34.5; // OK
end<type_list<std::string,char>>::result b = 'g'; // b is char
```

7. pop_front and pop_end
you can find the implementation of pop_front in "lesson_4.hpp". But before looking and copying the code there, i want you
to think how we can implement them. I didn't write pop_end and there is homework:
#### Try to write pop_front and pop_end




  
