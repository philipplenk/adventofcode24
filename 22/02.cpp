#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

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

	constexpr auto number_of_numbers = 2000;
	constexpr auto sequence_length = 4;
	constexpr auto final_factor = []()
	{
		int factor= 1;
		for(int i=1; i<sequence_length; ++i)
			factor*=20;
		return factor;
	}();

	std::unordered_map<int, long long> change_sequence_sums;
	for(long long n; std::cin>>n; )
	{
		std::array<int, sequence_length> changes{};
		int changes_position = 0;
		int encoded_seq = 0;
		
		std::unordered_set<int> seen;
		for(int i=0; i<number_of_numbers; ++i)
		{
			const auto previous = n;
			n = prune(mix(n, n*64));
			n = prune(mix(n, n/32));
			n = prune(mix(n, n*2048));

			encoded_seq-=final_factor*(encoded_seq/final_factor);
			changes[changes_position] = n%10 -previous%10;
			encoded_seq = encoded_seq*20 + (changes[changes_position]+10);
			changes_position = (changes_position+1)%sequence_length;

			if(!seen.contains(encoded_seq))
			{
				seen.insert(encoded_seq);
				change_sequence_sums[encoded_seq]+=n%10;
			}
		}
	}

	std::cout<<std::ranges::max_element(change_sequence_sums, [](const auto& lhs, const auto& rhs)
	{
		return lhs.second<rhs.second;
	})->second;

	return 0;
}
