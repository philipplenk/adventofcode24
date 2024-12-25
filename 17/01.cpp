#include <array>
#include <iostream>
#include <string>
#include <vector>

enum class instruction
{
	adv = 0,
	bxl = 1,
	bst = 2,
	jnz = 3,
	bxc = 4,
	out = 5,
	bdv = 6,
	cdv = 7
};

std::istream& operator>>(std::istream& in, instruction& ins)
{
	int value;
	in>>value;
	ins = instruction{value};
	return in;
}

int main(int argc, char* argv[])
{
	std::array<long long, 3> registers;

	std::string trash_s;
	char trash_c;

	std::cin>>trash_s>>trash_s>>registers[0];
	std::cin>>trash_s>>trash_s>>registers[1];
	std::cin>>trash_s>>trash_s>>registers[2];

	std::cin>>trash_s;

	std::vector<int> program;
	for(int value; std::cin>>value; std::cin>>trash_c)
		program.push_back(value);

	const auto combo = [&](long long value)
	{
		if(value<4) return value;
		if(value<7) return registers[value-4];
		return 0ll;
	};

	bool first_output = true;
	for(std::size_t pc = 0; pc<program.size(); pc+=2)
	{
		switch(instruction{program[pc]})
		{
			case instruction::adv: registers[0] = registers[0]>>(combo(program[pc+1])); break;
			case instruction::bxl: registers[1] = registers[1]^program[pc+1]; break;
			case instruction::bst: registers[1] = combo(program[pc+1])%8; break;
			case instruction::jnz:
			{
				if(registers[0]!=0)
				{
					pc = program[pc+1];
					pc-=2;
				}
				break;
			}
			case instruction::bxc: registers[1] = registers[1]^registers[2]; break;
			case instruction::out:
			{
				std::cout<<(first_output?"":",")<<(combo(program[pc+1])%8);
				first_output = false;
				break;
			}
			case instruction::bdv: registers[1] = registers[0]>>(combo(program[pc+1])); break;
			case instruction::cdv: registers[2] = registers[0]>>(combo(program[pc+1])); break;
		}
	}

	return 0;
}
