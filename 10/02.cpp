#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin},{}};
	const int width = grid[0].size();
	const int height = grid.size();

	struct point2d
	{
		int x, y;
	};

	std::vector<point2d> starting_positions;
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<height; ++x)
		{
			if(grid[y][x]=='0')
				starting_positions.push_back({x,y});
		}
	}

	int total = 0;
	for(const auto& start: starting_positions)
	{
		int score = 0;
		std::vector<point2d> to_visit;
		to_visit.push_back(start);
		while(!to_visit.empty())
		{
			const auto next = to_visit.back();
			to_visit.pop_back();

			if(grid[next.y][next.x]=='9')
			{
				++score;
				continue;
			}

			const auto try_add = [&](point2d p)
			{
				if(p.x<0 || p.y<0 || p.x>=width || p.y>=height) return;
				
				const auto current_elevation = grid[next.y][next.x];
				if(grid[p.y][p.x]!=current_elevation+1) return;

				to_visit.push_back(p);
			};

			try_add({next.x+1, next.y});
			try_add({next.x-1, next.y});
			try_add({next.x, next.y+1});
			try_add({next.x, next.y-1});
		}

		total+=score;
	}

	std::cout<<total;
	return 0;
}
