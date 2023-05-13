# name of C++ compiler
CXX       = g++
# options to C++ compiler
CXX_FLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -Werror
# flag to linker to make it link with math library
LDLIBS    = -lm
# list of object files
OBJS      = main.o data.o functions.o
# name of executable program
EXEC      = main.out

# by convention the default target (the target that is built when writing
# only make on the command line) should be called all and it should
# be the first target in a makefile
all : $(EXEC)

# however, the problem that arises with the previous rule is that make
# will think all is the name of the target file that should be created
# so, we tell make that all is not a file name
.PHONY : all

# this rule says that target $(EXEC) will be built if prerequisite
# files $(OBJS) have changed more recently than $(EXEC)
# text $(EXEC) will be replaced with value functions.out
# text $(OBJS) will be substituted with list of prerequisites in line 10
# line 31 says to build $(EXEC) using command $(CXX) with
# options $(CXX_FLAGS) specified on line 6
$(EXEC) : $(OBJS)
	$(CXX) $(CXX_FLAGS) $(OBJS) -o $(EXEC) $(LDLIBS)

# target main.o depends on both main.cpp, data.h, and functions.h
# and is created with command $(CXX) given the options $(CXX_FLAGS)
main.o : main.cpp data.h functions.h
	$(CXX) $(CXX_FLAGS) -c main.cpp -o main.o
	
# target data.o depends on both data.cpp and data.h
# and is created with command $(CXX) given the options $(CXX_FLAGS)
data.o : data.cpp data.h
	$(CXX) $(CXX_FLAGS) -c data.cpp -o data.o

# target functions.o depends on both functions.cpp and functions.h
# and is created with command $(CXX) given the options $(CXX_FLAGS)
functions.o : functions.cpp functions.h
	$(CXX) $(CXX_FLAGS) -c functions.cpp -o functions.o

# says that clean is not the name of a target file but simply the name for
# a recipe to be executed when an explicit request is made
.PHONY : clean
# clean is a target with no prerequisites;
# typing the command in the shell: make clean
# will only execute the command which is to delete the object files
clean :
	rm -f $(OBJS) $(EXEC)

# says that rebuild is not the name of a target file but simply the name
# for a recipe to be executed when an explicit request is made
.PHONY : rebuild
# rebuild is for starting over by removing cleaning up previous builds
# and starting a new one
rebuild :
	$(MAKE) clean
	$(MAKE)

.PHONY : test
test : $(EXEC)
	./$(EXEC) 0

test0 : $(EXEC)
	./$(EXEC) 0

test1 : $(EXEC)
	./$(EXEC) 1

test2 : $(EXEC)
	./$(EXEC) 2

test3 : $(EXEC)
	./$(EXEC) 3

test4 : $(EXEC)
	./$(EXEC) 4

test5 : $(EXEC)
	./$(EXEC) 5

test6 : $(EXEC)
	./$(EXEC) 6

test7 : $(EXEC)
	./$(EXEC) 7

test8 : $(EXEC)
	./$(EXEC) 8

test9 : $(EXEC)
	./$(EXEC) 9

test10 : $(EXEC)
	./$(EXEC) 10

.PHONY : leak
leak : $(EXEC)
	valgrind --leak-check=full ./$(EXEC) 0