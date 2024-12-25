#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<std::string> rows{std::istream_iterator<std::string>{std::cin},{}};
	const auto width = rows[0].size();
	const auto height = rows.size();

	std::vector<std::string> columns(width, std::string(height, ' '));
	std::vector<std::string> diagonals(width+height-1, std::string(width, ' '));
	std::vector<std::string> antidiagonals(width+height-1, std::string(width, ' '));

	for(std::size_t y=0; y<height; ++y)
	{
		for(std::size_t x=0; x<width; ++x)
		{
			columns[x][y] = rows[y][x];
			diagonals[x+y][x] = rows[y][x];
			antidiagonals[width-1-x+y][x] = rows[y][x];
		}
	}

	unsigned count = 0;
	for(const auto& lines: {rows, columns, diagonals, antidiagonals})
	{
		for(const auto& line: lines)
		{
			for(std::size_t i=0; i<line.size(); ++i)
			{
				const auto remainder = std::string_view{line}.substr(i);
				count+=remainder.starts_with("XMAS");
				count+=remainder.starts_with("SAMX");
			}
		}
	}
	
	std::cout<<count;
	return 0;
}
