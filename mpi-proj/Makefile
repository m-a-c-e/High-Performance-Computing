#VARIABLES
SRC_C		= hello.cpp
EXECUTABLE	= hello.out
E_OUT		= myscript.err
F_OUT		= output.txt
CC			= mpic++
CCFLAGS		= -Wall -O3
CLEAN_FILES = $(EXECUTABLEA) $(E_OUT) $(F_OUT) $(E_OUT)
PBS			= nqueens.pbs
NP			= 4


#RULES
$(EXECUTABLE): $(SRC_C)
	$(CC) -o $(EXECUTABLE) $(SRC_C)

run: $(EXECUTABLE)
	mpirun -np 1 ./$(EXECUTABLE)