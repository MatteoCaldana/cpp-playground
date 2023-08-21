CXX_FLAGS += -std=c++17 -Wall -I${mkEigenInc} -I${mkBoostInc} -L${mkBoostLib} -lboost_iostreams -lboost_system -lboost_filesystem

debug: main.cpp
	g++ -O0 -g -fsanitize=address ${CXX_FLAGS} main.cpp -o main_debug.out

release: main.cpp
	g++ -O3 -DNDEBUG ${CXX_FLAGS} main.cpp -o main_release.out

aggressive: main.cpp
	g++ -O3 -DNDEBUG -march=native -mavx -msse -ffast-math -funroll-loops ${CXX_FLAGS} main.cpp -o main_aggressive.out

distclean:
	rm -rf main_*