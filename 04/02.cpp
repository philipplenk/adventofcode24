#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<std::string> rows{std::istream_iterator<std::string>{std::cin},{}};
	const std::size_t width = rows[0].size();
	const std::size_t height = rows.size();

	std::vector<std::string> diagonals(width+height-1, std::string(width, ' '));
	std::vector<std::string> antidiagonals(width+height-1, std::string(width, ' '));

	for(std::size_t y=0; y<height; ++y)
	{
		for(std::size_t x=0; x<width; ++x)
		{
			diagonals[x+y][x] = rows[y][x];
			antidiagonals[width-1-x+y][x] = rows[y][x];
		}
	}

	std::vector<bool> centers(width*height, false);
	for(std::size_t id=1; id<diagonals.size(); ++id)
	{
		std::string_view diagonal = diagonals[id];
		const auto x_offset = id - std::min(height - 1, id);
		const auto y_offset = id - x_offset;
		const auto length = std::min(width-x_offset, y_offset + 1);

		for(std::size_t i=1; i+1<length; ++i)
		{
			const auto center_x = x_offset + i;
			const auto center_y = id - center_x;
			const auto remainder = diagonal.substr(center_x-1);
			centers[center_y*width+center_x] = remainder.starts_with("MAS") || remainder.starts_with("SAM");
		}
	}

	for(std::size_t id=1; id<antidiagonals.size(); ++id)
	{
		std::string_view antidiagonal = antidiagonals[id];
		const auto x_offset = width-1 - std::min(width-1, id);
		const auto y_offset = id + x_offset - (width-1);
		const auto length = std::min(width-x_offset, height-y_offset);

		for(std::size_t i=1; i+1<length; ++i)
		{
			const auto center_x = x_offset + i;
			const auto center_y = id + center_x - (width-1);
			const auto remainder = antidiagonal.substr(center_x-1);

			if(!remainder.starts_with("MAS") && !remainder.starts_with("SAM"))
				centers[center_y*width+center_x] = false;
		}
	}
	
	std::cout<<std::ranges::count(centers, true);
	return 0;
}
