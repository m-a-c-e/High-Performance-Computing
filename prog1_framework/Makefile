CXX=mpic++
CCFLAGS=-Wall -g
# activate for compiler optimizations:
#CCFLAGS=-Wall -O3
LDFLAGS=
PROCS=	2
SIZE=	4
DEPTH=	3
EXIT=	1

all: nqueen

nqueen: main.o solver.o utils.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c $<

run:
	mpirun -np $(PROCS) ./nqueen $(SIZE) $(DEPTH) $(EXIT)

clean:
	rm -f *.o nqueen
