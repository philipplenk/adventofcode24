#include <algorithm>
#include <array>
#include <limits>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstddef>

struct point2d
{
	int x, y;
};

struct input_grid
{
	constexpr input_grid(std::string_view data, int width, int height):
		data{data},width{width},height{height}
	{
		for(int y=0; y<height; ++y)
			for(int x=0; x<width; ++x)
				button_positions[data[y*width+x]] = {x,y};
	}

	constexpr auto key_at(const point2d& p) const
	{
		return data[p.y*width+p.x];
	}
	
	std::string_view data;
	int width, height;
	std::array<point2d, 256> button_positions{};
};

int main(int argc, char* argv[])
{
	constexpr int number_of_robots = 2;
	constexpr auto keypad = input_grid("789456123_0A", 3, 4);
	constexpr auto directional_keypad = input_grid("_^A<v>", 3, 2);	

	const auto enumerate_options = [&](this auto&& rec, point2d position, char next_command, const input_grid& pad)
	{
		const auto target = pad.button_positions[next_command];

		std::string commands;
		if(target.x<position.x) commands+=std::string(position.x-target.x,'<');
		if(target.x>position.x) commands+=std::string(target.x-position.x,'>');
		if(target.y<position.y) commands+=std::string(position.y-target.y,'^');
		if(target.y>position.y) commands+=std::string(target.y-position.y,'v');

		const auto is_valid = [&](const auto& sequence)
		{
			point2d current = position;
			for(auto c: sequence)
			{
				switch(c)
				{
					case '<': --current.x; break;
					case '>': ++current.x; break;
					case 'v': ++current.y; break;
					case '^': --current.y; break;
					default: break;
				}
				if(current.x<0 || current.y<0 || current.x>=pad.width || current.y>=pad.height) return false;
				if(pad.key_at(current)=='_') return false;
			}
			return true;
		};

		std::ranges::sort(commands);
		std::vector<std::string> valid_sequences;
		do
		{
			if(is_valid(commands))
			   valid_sequences.push_back(commands+'A');
		}
		while(std::next_permutation(commands.begin(), commands.end()));

		return valid_sequences;
	};

	std::unordered_map<std::size_t, std::size_t> cache;
	const auto find_sequence_length = [&](this auto&& rec, int robots, const std::vector<std::string>& codes)
	{
		if(robots==0) return codes.front().size();
		const auto& pad = robots==number_of_robots+1?keypad:directional_keypad;

		std::size_t minimum = std::numeric_limits<std::size_t>::max(); 
		for(const auto& code: codes)
		{
			std::size_t length = 0;
			for(char last = 'A'; auto c: code)
			{
				const auto cache_key = last*256 + c*256*256 + robots;
				if(cache.contains(cache_key))
					length+=cache[cache_key];
				else
					length+=(cache[cache_key] = rec(robots-1,enumerate_options(pad.button_positions[last], c, pad)));

				last = c;
			}
			minimum = std::min(length, minimum);
		}
		return minimum;
	};
	
	std::size_t sum = 0;
	for(std::string code; std::cin>>code; )
	{
		const auto numeric = 100*(code[0]-'0')+10*(code[1]-'0')+(code[2]-'0');
		const auto length = find_sequence_length(number_of_robots+1, {code});
		sum+=numeric*length;
	}
	
	std::cout<<sum;
	return 0;
}
