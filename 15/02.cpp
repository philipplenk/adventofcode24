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

int main(int argc, char* argv[])
{
	std::vector<std::string> grid;
	for(std::string line; std::getline(std::cin, line) && !line.empty(); )
	{
		std::string grid_line;
		for(auto c: line)
		{
			switch(c)
			{
				case 'O': grid_line+="[]"; break;
				case '@': grid_line+="@."; break;
				default: grid_line+=std::string(2,c); break;
			}
		}
		grid.push_back(grid_line);
	}

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

	const auto could_move = [&](this auto&& rec, point2d start, vec2d v)
	{
		const auto target = start + v;
		const auto to_push = grid[target.y][target.x];
		
		if(to_push=='#') return false;
		if(to_push=='.') return true;
		if(v.x!=0) return rec(start+v, v);

		const auto neighbour_offset = to_push=='['?vec2d{1,0}:vec2d{-1,0};
		return rec(start + v, v) && rec(start+v+neighbour_offset, v);
	};

	const auto try_move = [&](this auto&& rec, point2d start, vec2d v)
	{
		if(!could_move(start, v))
			return start;

		const auto target = start + v;
		const auto to_push = grid[target.y][target.x];
		
		if(to_push=='[' || to_push==']')
		{
			rec(start+v, v);
			if(v.x==0)
			{
				const auto neighbour_offset = to_push=='['?vec2d{1,0}:vec2d{-1,0};
				const auto neighbour = start+neighbour_offset+v;
				rec(neighbour, v);
				grid[neighbour.y][neighbour.x] = '.';
			}
		}

		grid[target.y][target.x] = grid[start.y][start.x];
		return target;
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
			if(grid[y][x]=='[')
			{
				result+=x+100*y;
			}
		}
	}

	std::cout<<result;
	return 0;
}
