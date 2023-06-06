CXX_FLAGS += -std=c++17 -Wall -pedantic -I${mkEigenInc}

debug: main.cpp
	g++ -O0 -g -fsanitize=address ${CXX_FLAGS} main.cpp -o main_debug

release: main.cpp
	g++ -O3 -DNDEBUG ${CXX_FLAGS} main.cpp -o main_release

aggressive: main.cpp
	g++ -O3 -DNDEBUG -march=native -mavx -msse -ffast-math -funroll-loops ${CXX_FLAGS} main.cpp -o main_aggressive

distclean:
	rm -rf main_*