#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	long long sum = 0;
	for(std::string line; std::getline(std::cin, line); )
	{
		std::stringstream strm{line};
		long long result;
		char skip;
		strm>>result>>skip;
		std::vector<long long> parameters{std::istream_iterator<long long>{strm},{}};
		
		const auto can_solve = [&](std::size_t idx, long long so_far, auto rec) -> bool
		{
			if(idx>=parameters.size())
				return so_far == result;

			if(so_far>result)
				return false;

			return rec(idx+1, so_far+parameters[idx], rec) || rec(idx+1, so_far*parameters[idx], rec) || rec(idx+1, std::stoll(std::to_string(so_far) + std::to_string(parameters[idx])), rec);
		};

		if(can_solve(1, parameters.front(), can_solve))
			sum+=result;
	}
	std::cout<<sum;
	return 0;
}
