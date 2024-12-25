#include <iostream>
#include <string>
#include <vector>

struct point2d
{
	int x, y;

	constexpr friend auto operator<=>(const point2d&, const point2d&) = default;
};

struct vec2d
{
	int x, y;
};

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
	std::vector<std::string> grid;
	for(std::string line; std::getline(std::cin, line) && !line.empty(); )
		grid.push_back(line);

	const int width = grid[0].size();
	const int height = grid.size();

	std::string moves;
	for(std::string line; std::getline(std::cin, line); )
	    moves+=line;

	point2d start{};
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(grid[y][x]=='@')
			{
				start = {x,y};
				grid[y][x]='.';
			}
		}
	}

	const auto try_move = [&](point2d current, vec2d v)
	{
		auto target = current + v;
		for(; grid[target.y][target.x]=='O'; target = target + v);

		if(grid[target.y][target.x]=='#') return current;
		while(target!=current)
		{
			const auto prev = target - v;
			grid[target.y][target.x] = grid[prev.y][prev.x];
			target = prev;
		}

		return current + v;
	};

	point2d current = start;
	for(auto move: moves)
	{
		switch(move)
		{
			case '<': current = try_move(current, {-1, 0}); break;
			case '>': current = try_move(current, {1, 0}); break;
			case '^': current = try_move(current, {0, -1}); break;
			case 'v': current = try_move(current, {0, 1}); break;
		}
	}

	long long result = 0;
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(grid[y][x]=='O')
				result+=x+100*y;
		}
	}

	std::cout<<result;
	return 0;
}
