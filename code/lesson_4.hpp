#include <typeinfo>
#include <tuple>

namespace aml {
	
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
		
	template<typename ...V>
	struct type_list{
		constexpr decltype(auto) operator=(const type_list<V...>& other){
			*this = other;
			return *this;
		}
	};

	template<typename ...P>
	constexpr bool empty(const type_list<P...>& d) {
		static constexpr type_list<> a;
		return (typeid(d) == typeid(a));
	}	

	template<typename First,typename ...Types>
	struct front;

	template<typename First,typename ...Types>
	struct front<type_list<First,Types...>> {
		using result = First;
	};
	

	template<typename F, typename ...Var>
	constexpr decltype(auto) pop_front(const type_list<F,Var...>& ls)
	{
		return type_list<Var...>();
	}

	template <typename ...V,typename ...D>
	constexpr decltype(auto) merge(const type_list<V...>& first, const type_list<D...>& second)
	{
		return type_list<V...,D...>();
	}

	template<typename First, typename ...Var>
	constexpr decltype(auto) reverse(type_list<First,Var...> ls)
	{ 
		return merge(reverse(type_list<Var...>()),type_list<First>());	
	}

	constexpr type_list<> reverse(type_list<> ls) {
		return type_list<>();	
	}

	template<typename First, typename ...Rest>
	struct end;

	template<typename First, typename ...Rest>
	struct end<type_list<First,Rest...>> {
		using result = front<decltype(reverse(type_list<First,Rest...>()))>::result;
	};

	
}; // namespace aml
