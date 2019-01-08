#include "sort_type.hpp"
int main()
{
	using tuple_t = std::tuple<short,int, char,double>;
	std::cout<<typeid(sort_type<tuple_t>::type).name()<<std::endl;
	std::cin.get();
	return 0;
}
