#pragma once
#include <iostream>
#include <tuple>
template<std::size_t...Indexs>
struct generator_indexs_From_To   //[m,n)
{
	using type = generator_indexs_From_To;
};

template<std::size_t From,std::size_t To,std::size_t...Indexs>
struct generator_indexs_From_To<From,To, Indexs...>
{
	using type = typename generator_indexs_From_To<From, To-1,To - 1, Indexs...>::type;
};

template<std::size_t From,std::size_t...Indexs>
struct generator_indexs_From_To<From,From,Indexs...>
{
	using type = generator_indexs_From_To<Indexs...>; 
};
template<bool Flag,typename First,typename Second>
auto swap_type(std::enable_if_t<Flag,void*> v= 0)
{
	return std::tuple<Second, First>{};
}

template<bool Flag, typename First, typename Second>
auto swap_type(std::enable_if_t<!Flag, void*> v = 0)
{
	return std::tuple<First,Second>{};
}

template<bool Flag,typename First,typename Second,std::size_t...Indexs, std::size_t...Indexs2,typename Tuple>
constexpr auto help_build_tuple(generator_indexs_From_To<Indexs...>, generator_indexs_From_To<Indexs2...>,Tuple&& tp)
{
	using swp = decltype(swap_type<Flag, First, Second>());
	return std::tuple<std::tuple_element_t<Indexs, Tuple>..., std::tuple_element_t<0, swp>, std::tuple_element_t<1, swp>, std::tuple_element_t<Indexs2, Tuple>...>{};
}

template<bool Flag, typename First, typename Second, std::size_t...Indexs2, typename Tuple>
constexpr auto help_build_tuple(generator_indexs_From_To<>, generator_indexs_From_To<Indexs2...>, Tuple&& tp)
{
	using swp = decltype(swap_type<Flag, First, Second>());
	return std::tuple<std::tuple_element_t<0, swp>, std::tuple_element_t<1, swp>, std::tuple_element_t<Indexs2, Tuple>...>{};
}

template<bool Flag, typename First, typename Second, std::size_t...Indexs,  typename Tuple>
constexpr auto help_build_tuple(generator_indexs_From_To<Indexs...>, generator_indexs_From_To<>, Tuple&& tp)
{
	using swp = decltype(swap_type<Flag, First, Second>());
	return std::tuple<std::tuple_element_t<Indexs, Tuple>..., std::tuple_element_t<0, swp>, std::tuple_element_t<1, swp>>{};
}


template<std::size_t O, std::size_t IMax,std::size_t I,typename Tuple>  // O = size-1     I [0 , size-1]    
struct sort_type_implement
{
	constexpr static auto call()
	{
		//std::cout << typeid(Tuple).name() <<"   "<<I<< std::endl;
		static_assert(std::tuple_size_v<Tuple> >= 2);
		using First = std::tuple_element_t<I, Tuple>;
		using Second = std::tuple_element_t<I+1, Tuple>;
		constexpr auto length = std::tuple_size_v<Tuple>;
		using first_seque = typename generator_indexs_From_To<0, I>::type;
		using second_seque = typename generator_indexs_From_To<I+2, length>::type;
		//std::cout << typeid(First).name() << "   " << typeid(Second).name() << std::endl;
		auto step_sort_tp = help_build_tuple<(sizeof(First)> sizeof(Second)), First, Second > (first_seque{}, second_seque{}, Tuple{});
		//std::cout << "result  " << typeid(step_sort_tp).name()<<"  "<< O <<"  "<<IMax<<"  "<<I+1<< std::endl;
		return sort_type_implement<O, IMax, I + 1, decltype(step_sort_tp)>::template call();
	}
};

template<typename Tuple>
struct sort_type_implement<1,1,0, Tuple>
{
	constexpr static auto call()
	{
		using First = std::tuple_element_t<0, Tuple>;
		using Second = std::tuple_element_t<1, Tuple>;
		auto tp = swap_type<(sizeof(First) > sizeof(Second)),First,Second>();
		return tp;
	}
};

template<std::size_t O, std::size_t IMax,  typename Tuple>
struct sort_type_implement<O, IMax, IMax, Tuple>
{
	constexpr static auto call()
	{
		return sort_type_implement<O - 1, IMax, 0, Tuple>::template call();
	}
};

template<std::size_t IMax, typename Tuple>
struct sort_type_implement<0, IMax, IMax, Tuple>
{
	static auto call()
	{
		return Tuple{};
	}
};

template<typename Tuple>
struct sort_type
{
	using type = decltype(sort_type_implement<std::tuple_size_v<Tuple>-1, std::tuple_size_v<Tuple>-1, 0, Tuple>::call());
};
