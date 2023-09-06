CC = g++
CFLAGS = -Wall -g -std=c++11

SRC_FILES = plug_reflect.cpp enigma.cpp rotor.cpp
BOMBE = bombe.cpp 
BRUTEFORCE = bruteforce.cpp
HEADER_FILES = $(wildcard *.hpp)
OBJ_FILES = $(wildcard *.o)
GCH_FILES = $(wildcard *.hpp.gch)

ARG = plugboard.pb reflectors/1.rf rotors/1.rot rotors/2.rot rotors/3.rot rotors/positions.pos < input.txt > 

bombe: $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(BOMBE) $(SRC_FILES) -o bombe
	 
	
run_bombe: bombe
	./bombe $(ARG) output_bombe.txt

bruteforce: $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(BRUTEFORCE) $(SRC_FILES) -o bruteforce
	 
	
run_bruteforce: bruteforce
	./bruteforce $(ARG) output_bruteforce.txt


clean:
	rm -f bombe bruteforce $(OBJ_FILES) $(GCH_FILES)

.PHONY: clean
