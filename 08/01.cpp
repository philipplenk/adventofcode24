#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstddef>

struct point2d
{
	int x, y;
};

struct vec2d
{
	int x, y;
};

vec2d operator-(const point2d& lhs, const point2d& rhs)
{
	return {lhs.x-rhs.x, lhs.y-rhs.y};
}

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.x, lhs.y+rhs.y};
}

point2d operator-(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x-rhs.x, lhs.y-rhs.y};
}

int main(int argc, char* argv[])
{
	
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin}, {}};
	const int width = grid[0].size();
	const int height = grid.size();
	
	std::unordered_map<char, std::vector<point2d>> antennae;
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(std::isalnum(grid[y][x]))
				antennae[grid[y][x]].push_back({x,y});
		}
	}

	const auto in_bounds = [&](point2d p)
	{
		return p.x>=0 && p.x<width && p.y>=0 && p.y<height;
	};

	std::vector<bool> antinodes(width*height, false);
	for(const auto& [type, positions]: antennae)
	{
		for(std::size_t i=0; i<positions.size(); ++i)
		{
			for(std::size_t j=i+1; j<positions.size(); ++j)
			{
				const auto p0 = positions[i];
				const auto p1 = positions[j];

				const auto step = p0 - p1;

				const auto first = p0 + step;
				const auto second = p1 - step;

				if(in_bounds(first))
					antinodes[first.y*width+first.x] = true;

				if(in_bounds(second))
					antinodes[second.y*width+second.x] = true;
			}
		}
	}

	std::cout<<std::ranges::count(antinodes, true);	
	return 0;
}
