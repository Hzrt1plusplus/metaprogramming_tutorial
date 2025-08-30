# included 
1. iostream -> for std::cout
2. tuple -> for std::index_sequence, std::make_index_sequence and std::get

If you don't know about these STL function and classes, see https://cppreference.com/ and learn about.

### Step 1:
What is a variadic template and in which cases it is useful? Let's consider we want to write a function that works like python's 
print() function. It will seem like(let's say name of this function is printfunc()):
``` C++
printfunc("Hello!",12,33,45.6); // OK
printfunc("C++"); // also OK
printfunc(); // just prints blank line 
```
You can see it works with printable(i.e. strings and numbers) objects. We will cover error handling and what is idea of printable 
in next lessons when we will learn about c++ concepts. For now, think about variadic templates as templates that takes multiple parameters 
where we don't know the size of params. 

### Step 2:
``` C++
void printfunc()
{
	std::cout << std::endl;
}
```

It is normal function and prints blank line and exists just for overloading real function. 
``` C++
template<typename first, typename ...rest>
void printfunc(first f, rest... params)
{
	std::cout << f << ", ";
	printfunc(params...);
}
```
In there you see ```typename ... rest``` which is variadic template definition. It means there are much more than one type. It prints first
variable and calls same function with ```printfunc(params...)```. ``` params``` is the rest of the parameters. 
When it will reach last element of params, function call will be something like: 
``` C++
{
  // inside of functuion
  std::cout << last_element-1 << ", ";
  printfunc(last_element); // params... is only last element 
}
```
In this case we have to write a funciton that ends recursion:
``` C++
template<typename LAST>
void printfunc(LAST t)
{
	std::cout << t << '\n';
}
```

So now, we have a pretty pythonic dynamic parametered print function. 
### Step 3:
Let's say we want to write a function it prints tuple:
``` C++
print_tuple(some_tuple); // output(maybe) : 1, 2, 3, "salam", 'g'
```
But how? First, write it:
``` C++
template<typename TUPLE>
void print_tuple(TUPLE t)
{
	print_tuple_real(t,std::make_index_sequence<std::tuple_size<TUPLE>::value>{});
}
```
The work is done inside ```print_tuple_real``` funciton:
``` C++
template<typename TUPLE, std::size_t ... indicies>
void print_tuple_real(TUPLE t, std::index_sequence<indicies...>)
{
	printfunc(std::get<indicies>(t)...);
}
```
Function ```print_tuple_real``` takes one tuple and std::index_sequence object. it is a variadic parameter too. It is simply a list of indexes inside of tuple.
Next, we use function ```printfunct``` with variadic parameters. When we call ```std::get<indicies>...``` it returns list of this variables in
tuple. And works nicely when you call:
``` C++
printtuple(some_tuple);
```
