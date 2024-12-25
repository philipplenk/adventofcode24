#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	const auto count = [&](this auto&& rec, long long stone, int steps) -> long long
	{
		if(steps==0) return 1;
	
		if(stone==0)
			return rec(1, steps-1);

		const auto as_str = std::to_string(stone);
		if(as_str.size()%2==0)
		{
			auto first = rec(std::stoll(as_str.substr(0,as_str.size()/2)), steps-1);
			return first + rec(std::stoll(as_str.substr(as_str.size()/2)), steps-1);
		}
		return rec(stone*2024, steps-1);
			
	};

	long long sum = 0;
	for(long long stone; std::cin>>stone;)
		sum+=count(stone, 25);
	std::cout<<sum;
	
	return 0;
}
