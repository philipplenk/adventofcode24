#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector<int> as;
	std::unordered_map<int, int> counts;
	for(int a, b; std::cin>>a>>b;)
	{
		as.push_back(a);
		++counts[b];
	}

	const auto sum = std::transform_reduce(as.begin(), as.end(), 0, std::plus<>{}, [&](auto v)
	{
		return v*counts[v];
	});

	std::cout<<sum;
}

