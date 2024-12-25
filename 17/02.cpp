#include <iostream>
#include <vector>

#include <cstdint>

/*
	The given program - arranged to pair operations and operands - was the following:

	2 4
	1 5
	7 5
	1 6
	4 1
	5 5
	0 3
	3 0

	"Disassembling" this yields:

	bst A
	bxl 5
	cdv B
	bxl 6
	bxc
	out B
	adv 3
	jnz

	Writing this in slightly more readable/common notation yields:

	do
		B = A%8
		B = B xor 5
		C = A>>B
		B = B xor 6
		B = B xor C
		print(B%8)
		A = A>>3
	while(A!=0) 

	At which point, we can notice a few important facts:

		0. The values of B and C are reassigned in each iteration and
		1. A is only modified in the last instruction, shifting it by 3 bits
			=> This implies
				-> all loop iterations are *independent*.
				-> A is 0 after n loop iterations, so it contains 3*n bits.

		2. Furthermore, we only ever shift A *to the right*, and only by (A%8) xor 5 meaning:
			-> the computation of our first character might depend on the lowest 10 bit (first 3 + up to 7 we got from shifting)
			-> the computation of our second character might depend on the bits 3 up to 3+10
			-> and so on. However:
			-> We know A has to be 0 after shifting it 3 bits to the right n times.
				=> As such: The final output *only* depends on the final 3 bits of A!

	As such, we can work backwards:

		- Figure out all possible 3 bit values for A that yield the last desired value
		- Shift those 3 to the left and try finding values for the next 3 bits to get the second to last value
		- For each one that succeeded with, shift the combination to the left and continue likewise

	Once the first such recursion reaches the first character, we have combined the lowest value for A
	which yields all the desired output!
*/

int main()
{

	std::vector<int> target{2,4,1,5,7,5,1,6,4,1,5,5,0,3,3,0};

	const auto solve_rec = [&](this auto&& rec, int idx, std::uint64_t prefix_a) -> void
	{
		if(idx<0)
		{
			std::cout<<prefix_a;
			std::exit(0);
		};
		
		for(int i=0; i<8; ++i)
		{
			std::uint64_t a = (prefix_a<<3) | i;
			std::uint64_t b = (a%8)^5;
			std::uint64_t c = a>>b;
			b = b ^ 6 ^ c;
			if((b%8)==target[idx])
				rec(idx-1, (prefix_a<<3) | i);
		}
	};

	solve_rec(target.size()-1, 0);
}
