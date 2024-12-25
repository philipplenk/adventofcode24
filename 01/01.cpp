#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include <cmath>

int main(int argc, char* argv[])
{
	std::vector<int> as, bs;
	for(int a, b; std::cin>>a>>b;)
	{
		as.push_back(a);
		bs.push_back(b);
	}
	std::ranges::sort(as);
	std::ranges::sort(bs);

	const auto sum = std::transform_reduce(as.begin(), as.end(), bs.begin(), 0, std::plus<>{}, [](int a, int b)
	{
		return std::abs(a-b);
	});
	std::cout<<sum;
}

