#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main(int argc, char* argv[])
{
	struct block_info
	{
		long offset, id, size;
	};
	std::vector<block_info> free, used;

	bool is_file = true;
	long offset = 0, id = 0;
	for(char c; std::cin>>c; is_file = !is_file)
	{
		if(is_file)
			used.push_back({offset, id++, c-'0'});
		else
			free.push_back({offset, id, c-'0'});

		offset+=c-'0';
	}

	long long checksum = 0;

	for(auto file: used | std::views::reverse)
	{
		const auto it = std::ranges::find_if(free, [&](auto v)
		{
			return v.offset<file.offset && v.size>=file.size;
		});

		if(it!=free.end())
		{
			const auto offset_sum = file.size*it->offset + (file.size-1)*file.size/2;
			checksum+=offset_sum*file.id;
			it->offset+=file.size;
			it->size-=file.size;
		}
		else
		{
			const auto offset_sum = file.size*file.offset + (file.size-1)*file.size/2;
			checksum+=offset_sum*file.id;
		}
	}

	std::cout<<checksum;
	return 0;
}
