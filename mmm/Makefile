CPPFLAGS := -std=c++20 -Wall -Wpedantic -I${mkOpenblasInc}
LDFLAGS := -L${mkOpenblasLib} -lopenblas

all: main.cpp
	g++ $(CPPFLAGS) $(LDFLAGS) -O0 -g main.cpp -o debug 
	g++ $(CPPFLAGS) $(LDFLAGS) -O2 -DNDEBUG main.cpp -o release
	g++ $(CPPFLAGS) $(LDFLAGS) -O3 -ffast-math -DNDEBUG -march=native -funroll-all-loops -msse2 -mavx main.cpp -o aggressive