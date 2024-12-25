#include <algorithm>
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

int main(int argc, char* argv[])
{
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
		point2d p;
		int steps;
	};

	const auto compare = [&](const waypoint& lhs, const waypoint& rhs)
	{
		const auto lhs_dist = std::abs(lhs.p.x-end.x)+std::abs(lhs.p.y-end.y);
		const auto rhs_dist = std::abs(rhs.p.x-end.x)+std::abs(rhs.p.y-end.y);

		return std::tie(lhs.steps, lhs_dist)>std::tie(rhs.steps, rhs_dist);
	};

	const auto collect = [&](point2d from, point2d to)
	{
		std::vector<int> distances(width*height, std::numeric_limits<int>::max());
		std::vector<bool> seen(width*height, false);
		std::priority_queue<waypoint, std::vector<waypoint>, decltype(compare)> to_visit{compare};
		to_visit.push({from, 0});

		while(!to_visit.empty())
		{
			const auto next = to_visit.top();
			to_visit.pop();

			distances[next.p.y*width+next.p.x] = std::min(next.steps, distances[next.p.y*width+next.p.x]);
			if(next.p==to)
			{
				return distances;
			}

			const auto try_add = [&](point2d p)
			{
				if(p.x<0 || p.y<0 || p.x>=width || p.y>=height)
					return;
				if(grid[p.y][p.x]=='#') return;

				if(seen[p.y*width+p.x]) return;
				seen[p.y*width+p.x] = true;

				to_visit.push({p, next.steps+1});
			};

			try_add({next.p.x+1, next.p.y});
			try_add({next.p.x-1, next.p.y});
			try_add({next.p.x, next.p.y+1});
			try_add({next.p.x, next.p.y-1});
		}
		return distances;
	};

	const auto from_start = collect(start, end);
	const auto to_end = collect(end, start);

	constexpr auto max_cheat = 2;
	constexpr auto desired_gain = 100;
	int count = 0;
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(grid[y][x]=='#') continue;

			for(int sy=std::max(0,y-max_cheat); sy<std::min(height,y+max_cheat+1); ++sy)
			{
				for(int sx=std::max(0,x-max_cheat); sx<std::min(width,x+max_cheat+1); ++sx)
				{
					if(grid[sy][sx]=='#') continue;

					const auto dist = std::abs(sx-x)+std::abs(sy-y);
					if(dist<=max_cheat)
					{
						if(from_start[y*width+x]+to_end[sy*width+sx]+dist+desired_gain<=from_start[end.y*width+end.x])
							++count;
					}
				}
			}
		}
	}

	std::cout<<count;
	return 0;
}
