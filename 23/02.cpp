#include <iostream>
#include <set>
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

	const auto intersection = [](const std::unordered_set<std::string>& lhs, const std::unordered_set<std::string>& rhs)
	{
		std::unordered_set<std::string> intersection;
		for(const auto& value: lhs)
			if(rhs.contains(value))
				intersection.insert(value);

		return intersection;
	};

	std::set<std::string> best_clique;
	const auto find_max_clique = [&](this auto&& rec, std::unordered_set<std::string> clique, std::unordered_set<std::string> include_maybe, std::unordered_set<std::string> exclude)
	{
		if(include_maybe.empty() && exclude.empty())
		{
			if(clique.size()>best_clique.size())
				best_clique = {clique.begin(), clique.end()};
			
			return;
		}

		for(const auto& to_try: include_maybe)
		{
			const auto& pivot = *include_maybe.begin();
			if(connections[pivot].contains(to_try)) continue;
			
			clique.insert(to_try);

			rec(clique, intersection(connections[to_try], include_maybe), intersection(connections[to_try], exclude));

			clique.erase(to_try);
			exclude.insert(to_try);
		}
	};

	std::unordered_set<std::string> all;
	for(const auto& [node, _]: connections) all.insert(node);
	find_max_clique({},all,{});
	
	for(bool first = true; const auto& node: best_clique)
	{
		std::cout<<(first?"":",")<<node;
		first = false;
	}
	
	return 0;
}
