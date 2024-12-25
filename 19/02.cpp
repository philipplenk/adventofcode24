#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

struct trie
{
	void insert(const std::string& str)
	{
		auto current = &root;
		for(auto c: str)
		{
			if(!current->children[c])
				current->children[c] = std::make_unique<node>();
			current = current->children[c].get();
		}
		++current->terminal_count;
	}
	
	struct node
	{
		std::array<std::unique_ptr<node>, 256> children;
		unsigned long terminal_count = 0;
	};
	node root;
};

int main(int argc, char* argv[])
{
	std::string pattern_line;
	std::getline(std::cin, pattern_line);

	std::stringstream strm{pattern_line};
	trie pattern_trie;
	for(std::string pattern; std::getline(strm, pattern, ','); strm>>std::ws)
		pattern_trie.insert(pattern);

	const auto possible_count = [&](std::string_view design)
	{
		std::unordered_map<const trie::node*, unsigned long> current;
		current[&pattern_trie.root] = 1;

		for(auto c: design)
		{
			std::unordered_map<const trie::node*, unsigned long> next;
			for(const auto& [node, count]: current)
			{
				if(node->children[c])
				{
					next[node->children[c].get()]+=count;
					next[&pattern_trie.root]+=count*node->children[c]->terminal_count;
				}
			}
			current = std::move(next);
		}

		unsigned long sum = 0;
		for(const auto& [node, count]: current)
			sum+=node->terminal_count*count;

		return sum;
	};

	unsigned long count = 0;
	for(std::string design; std::cin>>design;)
		count+=possible_count(design);

	std::cout<<count;
	return 0;
}
