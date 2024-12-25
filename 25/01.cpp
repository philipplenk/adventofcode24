#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<std::array<int,5>> locks, keys;
	while(std::cin)
	{
		const bool is_lock = std::cin.peek()=='#';
		
		std::array<int,5> sums{};
		for(std::string line; std::getline(std::cin, line) && !line.empty();)
		{
			for(std::size_t x=0; x<sums.size(); ++x)
				sums[x]+=line[x]=='#';
		}

		(is_lock?locks:keys).push_back(sums);
	}

	int count = 0;
	for(std::size_t key = 0; key<keys.size(); ++key)
	{
		for(std::size_t lock = 0; lock<locks.size(); ++lock)
		{
			bool ok = true;
			for(std::size_t x=0; x<locks[lock].size(); ++x)
				ok&=locks[lock][x]+keys[key][x]<=7;
			count+=ok;
		}
	}

	std::cout<<count;
	return 0;
}
