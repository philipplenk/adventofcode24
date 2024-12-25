#include <iostream>
#include <iterator>
#include <vector>

struct point2d
{
	int x, y;
};

struct vec2d
{
	int x, y;
};

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.x, lhs.y+rhs.y};
}

struct robot
{
	point2d position;
	vec2d velocity;
};

std::istream& operator>>(std::istream& in, robot& r)
{
	char trash_c;
	return in>>trash_c>>trash_c>>r.position.x>>trash_c>>r.position.y>>trash_c>>trash_c>>r.velocity.x>>trash_c>>r.velocity.y;
}

int main(int argc, char* argv[])
{
	constexpr auto width = 101;
	constexpr auto height = 103;
	
	std::vector<robot> robots{std::istream_iterator<robot>{std::cin},{}};

	for(int i=0; i<100; ++i)
	{
		for(auto& r: robots)
		{
			r.position = r.position+r.velocity;
			r.position.x = (r.position.x%width+width)%width;
			r.position.y = (r.position.y%height+height)%height;
		}
	}

	int q0 = 0, q1= 0, q2 = 0, q3 = 0;
	for(const auto& r: robots)
	{
		if(r.position.x<width/2 && r.position.y<height/2) ++q0;
		if(r.position.x<width/2 && r.position.y>height/2) ++q1;
		if(r.position.x>width/2 && r.position.y<height/2) ++q2;
		if(r.position.x>width/2 && r.position.y>height/2) ++q3;
	}

	std::cout<<q0*q1*q2*q3;
	return 0;
}
