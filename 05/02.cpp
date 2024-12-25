#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[])
{
	std::unordered_map<int, std::unordered_set<int>> after;
	for(std::string line; std::getline(std::cin, line) && !line.empty();)
	{
		std::stringstream strm{line};
		int first, second;
		char skip;
		strm>>first>>skip>>second;

		after[first].insert(second);
	}
	
	const auto ordered_before = [&](const auto& lhs, const auto& rhs)
	{
		return after[lhs].contains(rhs);
	};

	int sum = 0;
	for(std::string line; std::getline(std::cin, line);)
	{
		std::stringstream strm{line};
		char skip;

		std::vector<int> values;
		for(int value; strm>>value; strm>>skip)
			values.push_back(value);

		if(!std::ranges::is_sorted(values, ordered_before))
		{
			std::ranges::sort(values, ordered_before);
			sum+=values[values.size()/2];
		}
	}

	std::cout<<sum;
	return 0;
}
