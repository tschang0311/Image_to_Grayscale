.PHONY: clean

seq: hw6.h hw6.c grayseq.c
	clang -Wall -o grayseq hw6.c grayseq.c -pthread

par: hw6.h hw6.c graypar.c
	clang -Wall -o graypar hw6.c graypar.c -pthread

test: hw6.h hw6.c hw6-phase1-autograder.c
	clang -Wall -o test hw6.c hw6-phase1-autograder.c -pthread -lcriterion

clean:
	rm -rf a.out grayseq graypar test

