SOURCES = Stopwatch.cpp TTP.cpp main.cpp

all: main

main:
	g++ $(SOURCES) -o totallynotavirus -std=c++17

clean: 
	rm -rf totallynotavirus