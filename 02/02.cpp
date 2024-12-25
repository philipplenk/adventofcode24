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

		std::array<std::array<bool,3>,2> skipped{{{true, true, true}, {true, true, true}}};
		std::array<std::array<bool,3>,2> non_skipped{{{false, true, true}, {false, true, true}}};
		
		for(std::size_t i=1; i<values.size(); ++i)
		{
			if(i>1)
			{
				const auto diff = std::abs(values[i-2] - values[i]);
				const auto increasing = values[i-2]<values[i];
				const auto ok = (diff>=1 && diff<=3);
				
				skipped[increasing][0]|=(non_skipped[increasing][2] && ok);
			}
			const auto diff = std::abs(values[i-1] - values[i]);
			const auto increasing = values[i-1]<values[i];
			const auto ok = (diff>=1 && diff<=3);
			
			non_skipped[increasing][0] = (non_skipped[increasing][1] && ok);
			skipped[increasing][0] |= (skipped[increasing][1] && ok);

			for(int type = 0; type<2; ++type)
			{
				std::move(skipped[type].begin(), skipped[type].begin()+2, skipped[type].begin()+1);
				std::move(non_skipped[type].begin(), non_skipped[type].begin()+2, non_skipped[type].begin()+1);
				skipped[type][0] = non_skipped[type][0] = false;
			}
		}
		number_of_safe_entries += (non_skipped[0][2] || non_skipped[1][2] || skipped[0][1] || non_skipped[0][1] || skipped[1][1] || non_skipped[1][1]);
	}
	
	std::cout<<number_of_safe_entries;
	return 0;
}
