#include <algorithm>
#include <generator>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cstddef>
#include <cstdint>

/*
	It turns out the circuit given is supposed to be a simple ripple-carry adder, as described here: 
		https://en.wikipedia.org/wiki/Adder_(electronics)#Ripple-carry_adder

	In other words, z00 is computed as:
		
		x00 XOR y00

	and its carry as:

		x00 AND y00

	Every other output should be computed as:

		zN = (xN XOR yN) XOR carryN-1

	and its carry as:

		carryN = (xN AND yN) OR ( (xN XOR yN) AND carryN-1)

	My original solution simply printed a lot of debug info with which I did some manual analysis
	to figure out the incorrect wires. That was a fun little puzzle, but felt immensely unsatisfying
	as a solution.

	As such, here is a quick and dirty attempt at a more general solution that at least works for
	my input:

		- First, we try to identify the "variable names" used for each part of the above equations
			=> XOR with something else than x/y always is one of the outputs! (zN)
			=> OR is always computing a carry (and we can figure out which one by looking the arguments)
		- Once we have done so, we will inevitably notice a bunch of contradictions. So we collect
		  all wires involved in any contradiction and then simply try all selections of 4 pairs of those.

	To verify, I only use the value provided and a bunch of random trials, which is - of course -
	inadequate, stupid and can yield a great number of false positives.
	Instead, I should most likely construct the equations for each bit and check their correctness.
	Unfortunately, I am too tired and exhausted at the moment to properly think about any of that.
	Maybe another time...

	Soooo... certainly not the best possible approach (maybe the worst?), *but* it does work for my
	input and is "better" than manual analysis. Or so I tell myself.
	
	(An avid reader might have noticed a certain level of disgust in my tone. I do beg your forgiveness
	and hope you will not judge me or my abilities by this frightful abomination xD)
*/

struct connection
{
	std::string a, b, op, out;
};

std::istream& operator>>(std::istream& in, connection& c)
{
	std::string trash;
	return in>>c.a>>c.op>>c.b>>trash>>c.out;
}

auto evaluate(std::unordered_map<std::string, bool> variables, const std::vector<connection>& connections)
{
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

	return variables;
}

bool is_adding(std::unordered_map<std::string, bool> variables, const std::vector<connection>& connections)
{
	const auto extract_int = [&](char prefix)
	{
		std::uint64_t result = 0;
		for(const auto& [var, value]: variables)
		{
			if(var[0]==prefix)
				result|=std::uint64_t{value}<<std::stoul(var.substr(1));
		};
		return result;
	};

	const auto x = extract_int('x'); 
	const auto y = extract_int('y');

	variables = evaluate(std::move(variables), connections);

	const auto z = extract_int('z');

	return x+y==z;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, bool> variables;
	for(std::string line; std::getline(std::cin, line) && !line.empty();)
		variables[line.substr(0, line.size()-3)] = line.back()=='1';

	std::vector<connection> connections{std::istream_iterator<connection>{std::cin}, {}};

	std::unordered_set<std::string> potentially_incorrect_wires;

	/*
		Beware, huge unreadable blocks of code incoming. Here be dragons!
	*/

	std::vector<std::string> and_at(variables.size()/2);
	std::vector<std::string> xor_at(variables.size()/2);
	std::vector<std::string> carry_at(variables.size()/2);
	std::vector<std::string> xor_and_carry_at(variables.size()/2);

	/*
		First, collect all variable names used for "xN AND yN" as well as "xN XOR yN",
		as we need no additional information to identify those:
	*/
	std::unordered_map<std::string, int> is_xor_for;
	std::unordered_map<std::string, int> is_and_for;
	for(const auto& con: connections)
	{
		if(con.a.substr(1)==con.b.substr(1) && ((con.a.front()=='x' && con.b.front()=='y') || (con.a.front()=='y' && con.b.front()=='x')))
		{
			if(con.op=="AND")
			{
				and_at[std::stoi(con.a.substr(1))] = con.out;
				is_and_for[con.out] = std::stoi(con.a.substr(1));
			}
			else if(con.op=="XOR")
			{
				xor_at[std::stoi(con.a.substr(1))] = con.out;
				is_xor_for[con.out] = std::stoi(con.a.substr(1));
			}
		}
	}

	/*
		Alright, that was ugly. Anyway...
		We can now identify the carrys and which index they are a carry for in similarly
		disgusting fashion:
	*/
	std::unordered_map<std::string, int> is_carry_for;
	for(const auto& con: connections)
	{
		if((is_xor_for.contains(con.a) || is_xor_for.contains(con.b)) && con.op=="XOR")
		{
			int idx;
			std::string other;
			if(is_xor_for.contains(con.a))
			{
				idx = is_xor_for[con.a];
				other = con.b;
			}
			else
			{
				idx = is_xor_for[con.b];
				other = con.a;
			}

			/*
				There is huge potential for incorrect assignments here!
				In my case, 3 of the 4 groups could have been identified here alone, as they were
				simply XORs that did not yield a z...
				I don't want to assume the previous assignments are necessarily correct so, which is
				why I simply record the errors instead of trying to correct them on the spot:
			*/
			if(is_xor_for.contains(con.a) && is_xor_for.contains(con.b))
			{
				potentially_incorrect_wires.insert(con.a);
				potentially_incorrect_wires.insert(con.b);
			}
			
			if(con.out.front()!='z')
			{
				potentially_incorrect_wires.insert(con.out);
				potentially_incorrect_wires.insert('z'+((idx<10?"0":"")+std::to_string(idx)));
			}
			
			if(is_carry_for.contains(other))
			{
				potentially_incorrect_wires.insert(other);
			}
			
			if(!carry_at[idx-1].empty())
			{
				potentially_incorrect_wires.insert(con.out);
				potentially_incorrect_wires.insert(carry_at[idx-1]);
			}

			carry_at[idx-1] = other;
			is_carry_for[other] = idx-1;
		}
	}

	/*
		The horror continues, try to identify the variables used to store "(xN XOR yN) AND carryN-1":
	*/
	std::unordered_map<std::string, int> is_xor_and_carry_for; 
	for(const auto& con: connections)
	{
		if(is_carry_for.contains(con.out) && con.op=="OR")
		{
			const auto idx = is_carry_for[con.out];
			if(and_at[idx]==con.a)
			{
				is_xor_and_carry_for[con.b] = idx;
				xor_and_carry_at[idx] = con.b;
			}
			else if(and_at[idx]==con.b)
			{
				xor_and_carry_at[idx] = con.a;
				is_xor_and_carry_for[con.a] = idx;
			}
			else
			{
				potentially_incorrect_wires.insert(con.a);
				potentially_incorrect_wires.insert(con.b);
			}
		}
	}

	/*
		And finally, check for any contradictions in output or carry computations:
	*/
	for(const auto& con: connections)
	{
		if(con.out.front()=='z' && con.op=="XOR" && con.out!="z00")
		{
			const auto idx = std::stoi(con.out.substr(1));
			if(con.a==xor_at[idx] && con.b==carry_at[idx-1]) continue;
			if(con.b==xor_at[idx] && con.a==carry_at[idx-1]) continue;

			potentially_incorrect_wires.insert(con.a);
			potentially_incorrect_wires.insert(con.b);
			if(!xor_at[idx].empty()) potentially_incorrect_wires.insert(xor_at[idx]);
			if(!carry_at[idx-1].empty()) potentially_incorrect_wires.insert(carry_at[idx-1]);
		}

		if(is_xor_and_carry_for.contains(con.out) && con.op=="AND")
		{
			const auto idx = is_xor_and_carry_for[con.out];
			if(con.a==xor_at[idx] && con.b==carry_at[idx-1]) continue;
			if(con.b==xor_at[idx] && con.a==carry_at[idx-1]) continue;

			potentially_incorrect_wires.insert(con.a);
			potentially_incorrect_wires.insert(con.b);
			if(!xor_at[idx].empty()) potentially_incorrect_wires.insert(xor_at[idx]);
			if(!carry_at[idx-1].empty()) potentially_incorrect_wires.insert(carry_at[idx-1]);
		}
	}

	// Great, candidates are collected, now grab our pairings and experiment.
	// If I am doing this anyway, I might as well play around with coroutines a little,
	// as I have far too little practice with those :):

	const auto each_n = [](const auto& from, std::size_t n) -> std::generator<const std::vector<std::string>&>
	{
		std::vector<std::string> selection;

		const auto enumerate_rec = [&](this auto&& rec, auto next_it) -> std::generator<const std::vector<std::string>&>
		{
			if(selection.size()==n)
				co_yield selection;
			else
			{
				for(auto it = next_it; it!=from.end(); ++it)
				{
					selection.push_back(*it);
						co_yield std::ranges::elements_of(rec(std::next(it)));
					selection.pop_back();
				}
			}
		};

		co_yield std::ranges::elements_of(enumerate_rec(from.begin()));
	};

	const auto each_2_partition = [](const auto& from) -> std::generator<const std::vector<std::pair<std::string,std::string>>&>
	{
		std::vector<std::pair<std::string,std::string>> result;
		
		const auto enumerate_rec = [&](this auto&& rec, auto next_it) -> std::generator<const std::vector<std::pair<std::string,std::string>>&>
		{
			if(next_it==from.end())
				co_yield result;
			else
			{
				for(std::size_t i=0; i<result.size(); ++i)
				{
					if(result[i].second.empty())
					{
						result[i].second = *next_it;
							co_yield std::ranges::elements_of(rec(std::next(next_it)));
						result[i].second.clear();
					}
				}
				if(result.size()<from.size()/2)
				{
					result.push_back({*next_it, ""});
						co_yield std::ranges::elements_of(rec(std::next(next_it)));
					result.pop_back();
				}
			}
		};

		co_yield std::ranges::elements_of(enumerate_rec(from.begin()));
	};

	const auto check_with_swaps = [&](const auto& swaps)
	{
		auto fixed_connections = connections;
		for(auto& con: fixed_connections)
		{
			for(const auto& [a,b]: swaps)
			{
				if(con.out==a)
					con.out=b;
				else if(con.out==b)
					con.out=a;
			}
		}

		if(!is_adding(variables, fixed_connections))
			return false;

		// I am too lazy and tired to properly check, so instead, I shall fall back to random trials:
		std::mt19937 random_gen{1729};
		std::uniform_int_distribution<int> dist(0,1);
		for(int i=0; i<10; ++i)
		{
			for(int j=0; j<45; ++j)
			{
				const auto valX = dist(random_gen);
				const auto valY = dist(random_gen);
				variables["x"+((j<10?"0":"")+std::to_string(j))] = valX;
				variables["y"+((j<10?"0":"")+std::to_string(j))] = valY;
			}

			if(!is_adding(variables, fixed_connections))
				return false;
		}
			
		return true;
	};

	for(const auto& sel: each_n(potentially_incorrect_wires, 8))
	{
		for(const auto& assignment: each_2_partition(sel))
		{
			if(check_with_swaps(assignment))
			{
				std::vector<std::string> values;
				for(const auto& [a,b]: assignment)
				{
					values.push_back(a);
					values.push_back(b);
				}
				std::ranges::sort(values);
				for(bool first = true; const auto& v: values)
				{
					std::cout<<(first?"":",")<<v;
					first = false;
				}
				std::cout<<'\n';
			}
		}
	}
	
	return 0;
}
