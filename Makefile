OPT = -Wall -g
LOG = test2.log

TEST_PATH = ./testPgms/correct
TEST_I_PATH = ./testPgms/incorrect

.PHONY : all clean neat distclean

all : oz 
	
oz : structure.cpp ozutility.o SymTable.o
	g++ $(OPT)  structure.cpp ozutility.o SymTable.o  -o oz

SymTable.o : SymTable.h SymTable.cpp 
	g++ $(OPT) -c SymTable.cpp -o SymTable.o

ozutility.o : ozutility.h ozutility.cpp
	g++ $(OPT) -c ozutility.cpp -o ozutility.o

clean: 
	rm -f oz *.o

neat : 
	rm -f prog.cpp build_in.cpp func_build_in.cpp func_build_in.h mem.h wrapper.cpp retaddr.cpp

distclean : clean neat
	

ione : ./oz
	./oz $(TEST_I_PATH)/one.pgm 

itwo : ./oz
	./oz $(TEST_I_PATH)/two.pgm 

ithree : ./oz
	./oz $(TEST_I_PATH)/three.pgm 

ifour : ./oz
	./oz $(TEST_I_PATH)/four.pgm 

ifive : ./oz
	./oz $(TEST_I_PATH)/five.pgm 

isix : ./oz
	./oz $(TEST_I_PATH)/six.pgm 

iseven : ./oz
	./oz $(TEST_I_PATH)/seven.pgm 

one : ./oz
	./oz $(TEST_PATH)/one.pgm 

two : ./oz
	./oz $(TEST_PATH)/two.pgm 

three : ./oz
	./oz $(TEST_PATH)/three.pgm 

four : ./oz
	./oz $(TEST_PATH)/four.pgm

five : ./oz
	./oz $(TEST_PATH)/five.pgm 

six : ./oz
	./oz $(TEST_PATH)/six.pgm 

seven : ./oz
	./oz $(TEST_PATH)/seven.pgm 

eight : ./oz
	./oz $(TEST_PATH)/eight.pgm 

nine : ./oz
	./oz $(TEST_PATH)/nine.pgm 

ten : ./oz
	./oz $(TEST_PATH)/ten.pgm 

build_in: func_build_in.cpp   func_build_in.h 
	g++ $(OPT) -g -c func_build_in.cpp -o func_build_in.o
test : build_in prog.cpp retaddr.cpp
	g++ -g wrapper.cpp func_build_in.o -o wrapper.o
	./wrapper.o
