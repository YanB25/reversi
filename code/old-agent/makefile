all: game.out ../build/game4api.out ../build/game_benchmark.out
CC=g++
CCFLAGS=-std=c++11 -Ofast -D DEBUG -D SHOW_DROPPABLE
game.out: game.o blackwhite.o solution.o
	${CC} ${CCFLAGS} -o $@ $^
game.o: game.cpp solution.hpp blackwhite.hpp
	${CC} -c ${CCFLAGS} -o $@ $<
blackwhite.o: blackwhite.cpp blackwhite.hpp
	${CC} -c ${CCFLAGS} -o $@ $<
solution.o: solution.cpp solution.hpp
	${CC} -c ${CCFLAGS} -o $@ $<

clean:
	find . -name "*.o" -delete
	find . -name "*.out" -delete

game4api.o: game4api.cpp
	${CC} -c ${CCFLAGS} -o $@ $<
../build/game4api.out: game4api.o blackwhite.o solution.o
	${CC} ${CCFLAGS} -o $@ $^

game_benchmark.o: game_benchmark.cpp
	${CC} -c ${CCFLAGS} -o $@ $<
../build/game_benchmark.out: game_benchmark.o blackwhite.o solution.o
	${CC} ${CCFLAGS} -o $@ $^

run:
	./game.out