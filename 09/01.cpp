#include <iostream>
#include <ranges>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	struct block_info
	{
		long offset, id, size;
	};
	std::vector<block_info> free, files;

	bool is_file = true;
	long offset = 0, id = 0;
	for(char c; std::cin>>c; is_file = !is_file)
	{
		if(is_file)
			files.push_back({offset, id++, c-'0'});
		else
			free.push_back({offset, id, c-'0'});

		offset+=c-'0';
	}

	long long checksum = 0;
	for(std::size_t free_idx = 0; auto& file: files | std::views::reverse)
	{
		while(free_idx<free.size() && free[free_idx].offset<file.offset && file.size>0)
		{
			const auto moved = std::min(free[free_idx].size, file.size);
			const auto offset_sum = moved*free[free_idx].offset + (moved-1)*moved/2;
			checksum+=offset_sum*file.id;

			free[free_idx].offset+=moved;
			free[free_idx].size-=moved;
			file.size-=moved;

			if(free[free_idx].size==0)
				++free_idx;
		}

		if(file.size>0)
		{
			const auto offset_sum = file.size*file.offset + (file.size-1)*file.size/2;
			checksum+=offset_sum*file.id;
		}
	}

	std::cout<<checksum;
	return 0;
}
