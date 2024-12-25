#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct point2d
{
	long long x, y;
	friend auto operator<=>(const point2d&, const point2d&) = default;
};

struct vec2d
{
	long long x, y;
};

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.x, lhs.y+rhs.y};
}

vec2d operator*(const vec2d& lhs, long long rhs)
{
	return {lhs.x*rhs, lhs.y*rhs};
}

struct configuration
{
	vec2d a, b;
	point2d target;
};

std::istream& operator>>(std::istream& in, configuration& conf)
{
	std::string trash;
	char trash_c;
	in>>trash>>trash>>trash_c>>trash_c>>conf.a.x>>trash>>trash_c>>trash_c>>conf.a.y;
	in>>trash>>trash>>trash_c>>trash_c>>conf.b.x>>trash>>trash_c>>trash_c>>conf.b.y;
	in>>trash>>trash_c>>trash_c>>conf.target.x>>trash>>trash_c>>trash_c>>conf.target.y;

	conf.target.x+=10000000000000ll;
	conf.target.y+=10000000000000ll;
	return in;
}

int main(int argc, char* argv[])
{
	std::vector<configuration> confs{std::istream_iterator<configuration>{std::cin},{}};

	long long total = 0;

	for(const auto& conf: confs)
	{
//		x0*a + x1*b = xt
//		a = (xt-x1*b)/x0
		
//		b = (yt-y0*xt/x0)/(y1-y0*x1/x0)
//		b = (x0*yt - y0*xt)/(x0*y1-y0*x1)
		
		const auto to_divide = (conf.a.x*conf.target.y) - (conf.a.y*conf.target.x);
		const auto divide_by = (conf.a.x*conf.b.y)-(conf.a.y*conf.b.x);

		if(divide_by==0 || to_divide%divide_by!=0) continue;

		const auto b = to_divide/divide_by;
		const auto to_divide_a = conf.target.x - conf.b.x*b;
		if(conf.a.x==0 || to_divide_a%conf.a.x!=0) continue;
		const auto a = to_divide_a/conf.a.x;

		total+=a*3+b;
	}

	std::cout<<total;
	return 0;
}
