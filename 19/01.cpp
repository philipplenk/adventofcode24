#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

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
		current->is_terminal = true;
	}
	
	struct node
	{
		std::array<std::unique_ptr<node>, 256> children;
		bool is_terminal = false;
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

	const auto is_possible = [&](std::string_view design)
	{
		std::unordered_set<const trie::node*> current;
		current.insert(&pattern_trie.root);

		for(auto c: design)
		{
			std::unordered_set<const trie::node*> next;
			for(const auto& node: current)
			{
				if(node->children[c])
				{
					next.insert(node->children[c].get());
					if(node->children[c]->is_terminal)
						next.insert(&pattern_trie.root);
				}
			}
			current = std::move(next);
		}

		return std::ranges::any_of(current, [](const auto& node)
		{
			return node->is_terminal;
		});
	};

	int count = 0;
	for(std::string design; std::cin>>design;)
		count+=is_possible(design);

	std::cout<<count;
	return 0;
}
