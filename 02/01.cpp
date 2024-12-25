#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	int number_of_safe_entries = 0;
	for(std::string line; std::getline(std::cin, line);)
	{
		std::stringstream strm{line};
		std::vector<int> values{std::istream_iterator<int>{strm}, {}};
		
		const bool increasing = values[0]<values[1];
		bool is_safe = true;
		for(std::size_t i=0; i+1<values.size(); ++i)
		{
			const auto diff = std::abs(values[i+1] - values[i]);
			is_safe&= (diff>=1 && diff<=3);
			is_safe&= (values[i]<values[i+1]) == increasing;
		}
		number_of_safe_entries += is_safe;
	}
	
	std::cout<<number_of_safe_entries;
	return 0;
}
