#include <iostream>
#include <iterator>
#include <queue>
#include <tuple>
#include <vector>

struct point2d
{
	int x, y;
	constexpr friend auto operator<=>(const point2d&, const point2d&) = default;
};

std::istream& operator>>(std::istream& in, point2d& p)
{
	char skip;
	return in>>p.x>>skip>>p.y;
}

int main(int argc, char* argv[])
{
	constexpr auto width = 71;
	constexpr auto height = 71;
	const point2d target{width-1, height-1};
	std::vector<point2d> bytes{std::istream_iterator<point2d>{std::cin},{}};
	
	std::vector<bool> grid(width*height, false);
	for(int j=0; j<1024; ++j)
		grid[bytes[j].y*width+bytes[j].x] = true;

	std::vector<bool> seen(width*height, false);

	struct waypoint
	{
		point2d position;
		int steps;
	};
	
	const auto compare = [&](const auto& lhs, const auto& rhs)
	{
		const auto lhs_dist = std::abs(lhs.position.x-target.x)+std::abs(lhs.position.y-target.y);
		const auto rhs_dist = std::abs(rhs.position.x-target.x)+std::abs(rhs.position.y-target.y);
		return std::tie(lhs.steps,lhs_dist)>std::tie(rhs.steps,rhs_dist);
	};

	std::priority_queue<waypoint, std::vector<waypoint>, decltype(compare)> to_visit{compare};
	to_visit.push({{0,0},0});
	seen[0*width+0] = true;
	while(!to_visit.empty())
	{
		const auto next = to_visit.top();
		to_visit.pop();
		
		if(next.position==target)
		{
			std::cout<<next.steps;
			return 0;
		}

		const auto try_add = [&](point2d new_p)
		{
			if(new_p.x<0 || new_p.y<0 || new_p.x>=width || new_p.y>=height) return;
			if(grid[new_p.y*width+new_p.x]) return;
			if(seen[new_p.y*width+new_p.x]) return;
			seen[new_p.y*width+new_p.x] = true;
			to_visit.push({new_p, next.steps+1});
		};

		try_add({next.position.x+1, next.position.y});
		try_add({next.position.x-1, next.position.y});
		try_add({next.position.x, next.position.y+1});
		try_add({next.position.x, next.position.y-1});
	}

	return 0;
}
