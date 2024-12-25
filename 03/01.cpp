#include <iostream>
#include <optional>
#include <string_view>

#include <cctype>

int main(int argc, char* argv[])
{
	const auto check_and_consume = [](std::string_view str)
	{
		for(auto c: str)
		{
			if(std::cin.peek()!=c)
				return false;
			std::cin.get();
		}
		return true;
	};

	const auto try_parse_int = [](unsigned max_digits) -> std::optional<int>
	{
		if(!std::isdigit(std::cin.peek()))
			return std::nullopt;
		
		int result = 0;
		for(unsigned i=0; i<max_digits; ++i)
		{
			if(!std::isdigit(std::cin.peek()))
				break;

			result = result*10 + std::cin.get() - '0';
		}

		return result;
	};

	int sum = 0;
	for(char c; std::cin>>c;)
	{
		if(c!='m') continue;
		if(!check_and_consume("ul(")) continue;

		int product = 1;
		if(auto num = try_parse_int(3); num)
			product*=*num;
		else
			continue;

		if(!check_and_consume(",")) continue;
		
		if(auto num = try_parse_int(3); num)
			product*=*num;
		else
			continue;
		
		if(!check_and_consume(")")) continue;

		sum+=product;
	}
	
	std::cout<<sum;
	return 0;
}
