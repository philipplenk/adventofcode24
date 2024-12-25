#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[])
{
	constexpr auto bound = 75;
	
	std::unordered_map<long long, long long> cache;
	const auto count = [&](this auto&& rec, long long stone, int steps) -> long long
	{
		if(steps==0) return 1;
		
		const auto key = stone*(bound+1)+steps;
		if(cache.contains(key))
			return cache[key];
		
		if(stone==0)
			return cache[key] = rec(1, steps-1);

		const auto as_str = std::to_string(stone);
		if(as_str.size()%2==0)
		{
			auto first = rec(std::stoll(as_str.substr(0,as_str.size()/2)), steps-1);
			return cache[key] = first + rec(std::stoll(as_str.substr(as_str.size()/2)), steps-1);
		}
		return cache[key] = rec(stone*2024, steps-1);
	};

	long long sum = 0;
	for(long long stone; std::cin>>stone;)
		sum+=count(stone, bound);
	std::cout<<sum;
	
	return 0;
}
