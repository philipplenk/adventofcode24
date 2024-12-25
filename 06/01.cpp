#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct point2d
{
	int x, y;
	constexpr friend bool operator==(const point2d&, const point2d&) = default;
};

struct vec2d
{
	int x, y;
	constexpr friend bool operator==(const vec2d&, const vec2d&) = default;
};

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.x, lhs.y+rhs.y};
}

int main(int argc, char* argv[])
{
	constexpr vec2d up{0, -1}, down{0, 1}, left{-1, 0}, right{1, 0};
	
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin},{}};
	const int width = grid[0].size();
	const int height = grid.size();

	point2d starting_position{};
	vec2d starting_direction{};
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			switch(grid[y][x])
			{
				case '^':
				{
					starting_position = {x,y};
					starting_direction = up;
					break;
				}
				case 'v':
				{
					starting_position = {x,y};
					starting_direction = down;
					break;
				}
				case '<':
				{
					starting_position = {x,y};
					starting_direction = left;
					break;
				}
				case '>':
				{
					starting_position = {x,y};
					starting_direction = right;
					break;
				}
			}
		}
	}

	const auto turn_right = [](vec2d direction)
	{
		return vec2d{-direction.y, direction.x};
	};

	const auto is_inside = [&](const point2d& p)
	{
		return p.x>=0 && p.x<width && p.y>=0 && p.y<height;
	};
	
	const auto is_blocked = [&](const point2d& p)
	{
		if(!is_inside(p))
			return false;
		if(grid[p.y][p.x]=='#')
			return true;
		return false;
	};

	auto current_position = starting_position;
	auto current_direction = starting_direction;

	std::vector<bool> visited(width*height, false);
	while(is_inside(current_position))
	{
		visited[current_position.y*width+current_position.x] = true;

		auto next = current_position + current_direction;
		while(is_blocked(next))
		{
			current_direction = turn_right(current_direction);
			next = current_position + current_direction;
		}

		current_position = next;
	}

	std::cout<<std::ranges::count(visited, true);
	return 0;
}
