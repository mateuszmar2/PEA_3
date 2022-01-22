alias r := run
alias b := build

# only run
run:
    rlwrap ./Main

# compile and run
build:
	clear
	g++ -o Main src/Main.cpp src/Towns.cpp src/Genetic_Algorithm.cpp -O2
	just run

# run with valgrind
valgrind:
	clear
	g++ -g -o Main src/Main.cpp src/Towns.cpp src/Genetic_Algorithm.cpp -Wall
	valgrind --track-origins=yes ./Main

# run with valgrind and full check for leak
valgrindleak:
	clear
	g++ -g -o Main src/Main.cpp src/Towns.cpp src/Genetic_Algorithm.cpp -Wall
	valgrind --leak-check=full ./Main