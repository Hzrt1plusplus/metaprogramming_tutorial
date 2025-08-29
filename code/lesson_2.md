# included 
If you don't know why i use classes and functions of STL bellow, see https://cppreference.com/ cite for more information 
about STL functions for metaprogramming.
1. tuple       -> std::tuple, std::tuple_element_t, std::tuple_size
2. iostream    -> std::cout
3. vector      -> std::vector
4. stdexcept   -> std::runtime_error
5. string      -> std::string
6. type_traits -> std::is_same, std::false_type, std::true_type

At the end of this tutorial we will have a function which returns if the type is in the list of types of a tuple.

### Step 1:
I write a toy function which is just a prototype of actual contains_type metafunction:
``` C++
bool contains(const std::string& search,const std::vector<std::string>& words, size_t start_from = 0)
{
	if (start_from >= words.size())
		throw std::runtime_error(
			"Error, no such index!"
		);
	if(words[start_from] == search)
		return true;
	else {
		if (start_from  == words.size()-1) return false;
		else return contains(search,words,start_from+1);
	}
}
```

you can call:
``` C++ 
std::vector<std::string> example{"int","float","bool"};
contains("int",example); //  returns true
contains("char",example); // returns false
contains("string",{"int","char","bool"}); // returns false
// and so on ...
```
Main idea behind this funciton is our actual function is going recursive and works like this one. So think about the work principle 
of this function.

### Step 2:
Before writing actual function we need something(a functor object -> i.e. a struct that behaves like a function) which is intended 
to do type deduction:

``` C++
template<bool condition, typename THEN, typename ELSE>
struct if_; // if true, then the type is THEN if no type is ELSE


template<typename THEN, typename ELSE>
struct if_<true,THEN,ELSE>	{
	using type = THEN; // if condition is true set it as type
};

template<typename THEN, typename ELSE>
struct if_<false,THEN,ELSE> {
	using type = ELSE; // if condition is false set it as else
};

```

You can use it like: 

``` C++
if (decltype(some_variable) == if_<other_condition,int,float>::type ){
  // do something
}
```
or more advanced:

``` C++
if_<condition,int,bool>::type variable; // the type of variable depends on the condition :)
// if condition is true, it will be int, else it will be bool
```
### Step 3:

``` C++

template<typename SEARCH, typename TUPLE, size_t index = 0>
struct contains_type : if_ <
	std::is_same<std::tuple_element_t<index,TUPLE>,SEARCH>::value,
	// THEN 
	std::true_type, 
	// ELSE 
	typename if_ <
		(index == std::tuple_size<TUPLE>::value),
		// THEN 
		std::false_type,
		// ELSE 
		contains_type<SEARCH,TUPLE,index+1>
	>::type
>::type
{};

```
First, you can see we use inheritance. We inherit our functor from two structs of STL(std::true_type and std::false_type). They are 
only useful for cases like this. The alternative definition of these structs are:
``` C++
struct true_type {
  static constexpr bool value = true;
);

struct false_type {
  static constexpr bool value = false;
};

```
At the end, we will use only value of this functor object which is merely a boolean. Let me explain: 
``` C++
struct contains_type : if_ <
	std::is_same<std::tuple_element_t<index,TUPLE>,SEARCH>::value,
  // THEN 
	std::true_type, 
```
the value in the second line is a boolean and it checks if the type of the parameter in the list of types in tuple type list(for example 
if we have a variable std::tuple<int,float,std::string> my_tuple,in this case the call of std::tuple_element_t<0,my_tuple> will return int
or the call of std::tuple_element_t<1,my_tuple> will return float) is same as our SEARCH type. if it is so, it will set ``` type ```
member of if_ as std::true_type and finally our contains_type object will inherit from this struct.  

``` C++
  // ELSE 
	typename if_ <
		(index == std::tuple_size<TUPLE>::value),
		// THEN 
		std::false_type,
		// ELSE 
		contains_type<SEARCH,TUPLE,index+1>
	>::type
>::type
{}; // end of contains_type 
```
I write ``` template ``` in front of ```if_``` in order to warn the compiler that the return value of if_ object is a type name. Otherwise 
compliler will raise an error. This if_ checks for statment (index == std::tuple_size<TUPLE>::value). tuple_size is a STL function 
which sets ```value``` member variable the size of TUPLE(note: TUPLE is an auto variable. It is neither a spesific type nor a variable,
it is just the return value of decltype(some_tuple)). If the condition is true, it means we are at the end of tuple's type list and 
the search failed and as a result it sets ```type``` member variable false_type(using type = THEN and in there THEN is std::false_type). 
if it is not so, it will call the functor object with search index  = index+1 again(contains_type<SEARCH,TUPLE,index+1>). In order to avoid 
possible errors i write another definition of contains_type for empty tuples: 
``` C++
template<typename SEARCH>
struct contains_type<SEARCH,std::tuple<>,0> : std::false_type {};
```

### Step 4:
Now let's call our function: 
``` C++
int main()
{
	std::tuple<int, float> t{12,12.2f};
	std::cout << std::boolalpha << contains_type<int,decltype(t)>::value << ' ' 
		      << contains_type<bool,std::tuple<>>::value << '\n';
	return 0;
}
```

Output: 
``` plaintext
true false
```



