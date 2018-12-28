all: test inheritage.out
CC=g++
CCFLAGS=-std=c++11 -ofast -D DEBUG -pthread
chessbox.o: chessbox.cpp chessbox.hpp
	${CC} -c ${CCFLAGS} -o $@ $<
alphabet.o: alphabet.cpp alphabet.hpp
	${CC} -c ${CCFLAGS} -o $@ $<
eval.o: eval.cpp eval.hpp
	${CC} -c ${CCFLAGS} -o $@ $<

test: chessbox_test

chessbox_test: test/test_chessbox.out

test/test_chessbox.out: test/test_chessbox.o chessbox.o alphabet.o eval.o
	${CC} ${CCFLAGS} -o $@ $^
test/test_chessbox.o: test/test_chessbox.cpp chessbox.hpp
	${CC} -c ${CCFLAGS} -o $@ $<

inheritage.out: inheritage.o chessbox.o alphabet.o eval.o
	${CC} ${CCFLAGS} -o $@ $^
inheritage.o: inheritage.cpp
	${CC} -c ${CCFLAGS} -o $@ $<

clean:
	find . -name "*.o" -delete
	find . -name "*.out" -delete

int:
	./inheritage.out

run:
	./test/test_chessbox.out
