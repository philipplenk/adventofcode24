#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	struct point2d
	{
		int x, y;
	};
	
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin}, {}};
	const int width = grid[0].size();
	const int height = grid.size();

	std::vector<bool> visited(width*height, false);

	const auto fill = [&](point2d start)
	{
		const auto type = grid[start.y][start.x];

		if(visited[start.y*width+start.x]) return 0;
		visited[start.y*width+start.x] = true;
		
		std::vector<point2d> to_visit;
		to_visit.push_back(start);

		int perimeter = 0, area = 0;
		while(!to_visit.empty())
		{
			const auto try_add = [&](point2d p)
			{
				if(p.x<0 || p.x>=width || p.y<0 || p.y>=height)
				{
					++perimeter;
					return;
				}
				if(grid[p.y][p.x]!=type)
				{
					++perimeter;
					return;
				}
				if(visited[p.y*width+p.x]) return;
				visited[p.y*width+p.x] = true;
				to_visit.push_back(p);
			};

			const auto next = to_visit.back();
			to_visit.pop_back();

			++area;
			try_add({next.x+1, next.y});
			try_add({next.x-1, next.y});
			try_add({next.x, next.y+1});
			try_add({next.x, next.y-1});
		}
		return area*perimeter;
	};

	int result = 0;
	for(int y=0;y<height; ++y)
		for(int x=0; x<width; ++x)
			result+=fill({x,y});

	std::cout<<result;
	return 0;
}
