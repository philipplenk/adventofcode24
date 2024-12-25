CFLAGS				=	-Wfatal-errors -std=c++23 -Wall -pedantic -Werror -O3

INCLUDE_PATH		=	-I../common
LIBS				=

all: 01
	
%: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE_PATH) $(LIBS) $< -o $@
	
clean:
	rm -f 01 02
	
run1: 01
	cat input | ./01
	
submit1: 01
	submit_result.sh 1 $(shell cat input | ./01)

.PHONY: clean run1 submit1 
