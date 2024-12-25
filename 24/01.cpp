#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstdint>

struct connection
{
	std::string a, b, op, out;
};

std::istream& operator>>(std::istream& in, connection& c)
{
	std::string trash;
	return in>>c.a>>c.op>>c.b>>trash>>c.out;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, bool> variables;
	for(std::string line; std::getline(std::cin, line) && !line.empty();)
		variables[line.substr(0, line.size()-3)] = line.back()=='1';

	std::vector<connection> connections{std::istream_iterator<connection>{std::cin}, {}};

	for(bool any = true; any;)
	{
		any = false;
		for(const auto& con: connections)
		{
			if(variables.contains(con.out)) continue;
			if(!variables.contains(con.a) || !variables.contains(con.b)) continue;

			any = true;
			if(con.op=="AND") variables[con.out] = variables[con.a] && variables[con.b];
			if(con.op=="OR") variables[con.out] = variables[con.a] || variables[con.b];
			if(con.op=="XOR") variables[con.out] = variables[con.a] ^ variables[con.b];
		}
	}

	std::uint64_t result = 0;
	for(const auto& [var, value]: variables)
	{
		if(var[0]=='z')
			result|=std::uint64_t{value}<<std::stoul(var.substr(1));
	}
	std::cout<<result;
	
	return 0;
}
