#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, std::unordered_set<std::string>> connections;

	for(std::string a, b; std::getline(std::cin, a, '-') && std::getline(std::cin, b); )
	{
		connections[a].insert(b);
		connections[b].insert(a);
	}

	int count = 0;
	std::unordered_set<std::string> seen; 
	for(const auto& [a, connected_to]: connections)
	{
		if(a.front()!='t') continue;
		
		for(auto it0 = connected_to.begin(); it0!=connected_to.end(); ++it0)
		{
			for(auto it1 = std::next(it0); it1!=connected_to.end(); ++it1)
			{
				if(!seen.contains(*it0) && !seen.contains(*it1) && connections.find(*it0)->second.contains(*it1))
					++count;
			}
		}
		seen.insert(a);
	}

	std::cout<<count;
	return 0;
}
