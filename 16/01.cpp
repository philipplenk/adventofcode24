#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
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
	constexpr friend auto operator<=>(const vec2d&, const vec2d&) = default;
};

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.x, lhs.y+rhs.y};
}

int main(int argc, char* argv[])
{
	constexpr vec2d up{0, -1}, down{0, 1}, left{-1, 0}, right{1, 0};
	
	const auto direction_to_index = [&](const vec2d v)
	{
		if(v==up) return 0; 
		if(v==down) return 1; 
		if(v==left) return 2; 
		return 3;
	};

	const auto turn_right = [](vec2d v)
	{
		return vec2d{v.y, -v.x};
	};

	const auto turn_left = [](vec2d v)
	{
		return vec2d{-v.y, v.x};
	};

	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin},{}};
	const int width = grid[0].size();
	const int height = grid.size();

	point2d start{}, end{};
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(grid[y][x]=='S') start = {x, y};
			if(grid[y][x]=='E') end = {x, y};
		}
	}

	struct waypoint
	{
		point2d position;
		vec2d direction;
		int cost = 0;
	};

	const auto compare = [&](const waypoint& lhs, const waypoint& rhs)
	{
		if(lhs.cost>rhs.cost) return true;
		if(lhs.cost<rhs.cost) return false;

		const auto lhs_dist = std::abs(lhs.position.x-end.x)+std::abs(lhs.position.y-end.y);
		const auto rhs_dist = std::abs(rhs.position.x-end.x)+std::abs(rhs.position.y-end.y);

		return lhs_dist>rhs_dist;
	};

	std::priority_queue<waypoint, std::vector<waypoint>, decltype(compare)> to_visit(compare);
	to_visit.push({start, right, 0});

	std::array<std::vector<int>,4> visited;
	for(auto& v: visited)
		v.resize(width*height, std::numeric_limits<int>::max());

	while(!to_visit.empty())
	{
		const auto next = to_visit.top();
		to_visit.pop();

		if(next.position==end)
		{
			std::cout<<next.cost;
			break;
		}

		const auto try_add = [&](waypoint wp)
		{
			if(wp.position.x<0 || wp.position.y<0 || wp.position.x>=width || wp.position.y>=height) return;
			if(grid[wp.position.y][wp.position.x]=='#') return;

			const auto idx = direction_to_index(wp.direction);
			if(visited[idx][wp.position.y*width+wp.position.x]<=wp.cost) return;
			visited[idx][wp.position.y*width+wp.position.x] = wp.cost;
			to_visit.push(wp);
		};

		try_add({{next.position+next.direction}, next.direction, next.cost+1});
		try_add({next.position, turn_left(next.direction), next.cost+1000});
		try_add({next.position, turn_right(next.direction), next.cost+1000});
	}

	return 0;
}
