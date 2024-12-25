#include <iostream>

int main(int argc, char* argv[])
{
	constexpr auto mod = 16777216;
	const auto mix = [](auto a, auto b)
	{
		return a^b;
	};

	const auto prune = [](auto v)
	{
		return v%mod;
	};

	long long sum = 0;
	for(long long n; std::cin>>n; )
	{
		for(int i=0; i<2000; ++i)
		{
			n = prune(mix(n, n*64));
			n = prune(mix(n, n/32));
			n = prune(mix(n, n*2048));
		}
		sum+=n;
	}
	std::cout<<sum;
	return 0;
}
