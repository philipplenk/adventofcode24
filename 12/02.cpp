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

		std::vector<bool> left_edges(width*height, false), right_edges(width*height, false);
		std::vector<bool> top_edges(width*height, false), bottom_edges(width*height, false);
		int area = 0;
		while(!to_visit.empty())
		{
			const auto next = to_visit.back();
			to_visit.pop_back();
			const auto try_add = [&](point2d p, auto& edges)
			{
				if(p.x<0 || p.x>=width || p.y<0 || p.y>=height)
				{
					edges[next.y*width+next.x] = true;
					return;
				}
				if(grid[p.y][p.x]!=type)
				{
					edges[next.y*width+next.x] = true;
					return;
				}
				if(visited[p.y*width+p.x]) return;
				visited[p.y*width+p.x] = true;
				to_visit.push_back(p);
			};

			++area;
			try_add({next.x+1, next.y}, right_edges);
			try_add({next.x-1, next.y}, left_edges);
			try_add({next.x, next.y+1}, bottom_edges);
			try_add({next.x, next.y-1}, top_edges);
		}

		int sides = 0;
		for(const auto& edges: {top_edges, bottom_edges})
		{
			for(int y=0;y<height; ++y)
			{
				for(int x=0; x<width; ++x)
				{
					if(edges[y*width+x])
					{
						++sides;
						while(x<width && edges[y*width+x]) ++x;
						--x;
					}
				}
			}
		}
		
		for(const auto& edges: {left_edges, right_edges})
		{
			for(int x=0; x<width; ++x)
			{
				for(int y=0;y<height; ++y)
				{
					if(edges[y*width+x])
					{
						++sides;
						while(y<height && edges[y*width+x]) ++y;
						--y;
					}
				}
			}
		}

		return area*sides;
	};

	int result = 0;
	for(int y=0;y<height; ++y)
		for(int x=0; x<width; ++x)
			result+=fill({x,y});
		
	std::cout<<result;
	return 0;
}
