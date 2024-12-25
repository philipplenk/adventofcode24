#include <iostream>
#include <iterator>
#include <queue>
#include <tuple>
#include <vector>

#include <cstddef>

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

std::ostream& operator<<(std::ostream& out, const point2d& p)
{
	return out<<p.x<<','<<p.y;
}

int main(int argc, char* argv[])
{
	constexpr auto width = 71;
	constexpr auto height = 71;
	const point2d target{width-1, height-1};
	std::vector<point2d> bytes{std::istream_iterator<point2d>{std::cin},{}};

	const auto max_time = width*height;
	std::vector<std::size_t> blocked_after(width*height, max_time);
	for(std::size_t i=0; i<bytes.size(); ++i)
		blocked_after[bytes[i].y*width+bytes[i].x] = i;

	struct waypoint
	{
		point2d position;
		std::size_t time;
	};

	const auto compare = [&](const auto& lhs, const auto& rhs)
	{
		const auto lhs_dist = std::abs(lhs.position.x-target.x)+std::abs(lhs.position.y-target.y);
		const auto rhs_dist = std::abs(rhs.position.x-target.x)+std::abs(rhs.position.y-target.y);
		return std::tie(rhs.time, lhs_dist)>std::tie(lhs.time, rhs_dist);
	};

	std::vector<std::size_t> seen_at(width*height, 0);
	std::priority_queue<waypoint, std::vector<waypoint>, decltype(compare)> to_visit{compare};
	to_visit.push({{0,0}, max_time});
	while(!to_visit.empty())
	{
		const auto next = to_visit.top();
		to_visit.pop();

		if(next.position==target)
			break;

		const auto try_add = [&](point2d new_p)
		{
			if(new_p.x<0 || new_p.y<0 || new_p.x>=width || new_p.y>=height) return;

			const auto time = std::min(next.time, blocked_after[new_p.y*width+new_p.x]);
			if(seen_at[new_p.y*width+new_p.x]>=time) return;
			seen_at[new_p.y*width+new_p.x] = time;
			to_visit.push({new_p,time});
		};

		try_add({next.position.x+1, next.position.y});
		try_add({next.position.x-1, next.position.y});
		try_add({next.position.x, next.position.y+1});
		try_add({next.position.x, next.position.y-1});
	}

	const auto last_time = seen_at[target.y*width+target.x];
	std::cout<<bytes[last_time];
	
	return 0;
}
