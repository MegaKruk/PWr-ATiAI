SOURCES = Stopwatch.cpp TTP.cpp main.cpp GreedySearch.cpp Item.cpp Knapsack.cpp SA.cpp GA.cpp

all: main

main:
	g++ $(SOURCES) -o totallynotavirus -std=c++17

clean: 
	rm -rf totallynotavirus