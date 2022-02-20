CXX=mpic++
CCFLAGS=-Wall -g --std=c++17 -O3
LDFLAGS=

SIZE=	14
PROCS=	8
DEPTH=	4
EXIT=	1

all: nqueen

nqueen: main.o solver.o utils.o
	$(CXX) $(LDFLAGS) $(CCFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c $<

run:
	mpirun -np $(PROCS) ./nqueen $(SIZE) $(DEPTH) $(EXIT)

clean:
	rm -f *.o nqueen
