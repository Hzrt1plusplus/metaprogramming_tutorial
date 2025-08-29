#include <tuple>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <type_traits>

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

template<bool condition, typename THEN, typename ELSE>
struct if_; // if true, then the type is THEN if no type is ELSE 

template<typename THEN, typename ELSE>
struct if_<true,THEN,ELSE>	{
	using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false,THEN,ELSE> {
	using type = ELSE;
};

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

template<typename SEARCH>
struct contains_type<SEARCH,std::tuple<>,0> : std::false_type {};

int main()
{
	std::tuple<int, float> t{12,12.2f};
	std::cout << std::boolalpha << contains_type<int,decltype(t)>::value << ' ' 
		      << contains_type<bool,std::tuple<>>::value << '\n';
	return 0;
}
