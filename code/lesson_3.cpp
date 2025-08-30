// Hazret Hasanov 2025 
// Lesson 3: Variadic Templates 

#include <iostream>
#include <tuple>

 void printfunc()
{
	std::cout << std::endl;
}


template<typename LAST>
void printfunc(LAST t)
{
	std::cout << t << '\n';
}

template<typename first, typename ...rest>
void printfunc(first f, rest... params)
{
	std::cout << f << ", ";
	printfunc(params...);
}

template<typename TUPLE, std::size_t ... indicies>
void print_tuple_real(TUPLE t, std::index_sequence<indicies...>)
{
	printfunc(std::get<indicies>(t)...);
}

template<typename TUPLE>
void print_tuple(TUPLE t)
{
	print_tuple_real(t,std::make_index_sequence<std::tuple_size<TUPLE>::value>{});
}



int main()
{
	printfunc("salam",12,23,34.5);
	std::tuple<int,int,bool,std::string> sequ{12,33,true,"Salam!"};
	print_tuple(sequ);
	return 0;
}
