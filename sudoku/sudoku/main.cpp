#include <vector>
#include <fstream>

#include "sudoku.hpp"
#include "timer.hpp"

int main() {
   std::ifstream infile("hardest.txt");
   std::vector<std::string> sudokus;
   std::string line;

   while(getline(infile, line))
      sudokus.push_back(line);

   Chrono t;
   t.reset();
   for(const auto sudoku : sudokus) {
      Sudoku S(sudoku);
      S.solve(S);
   }
   std::cout << t.split() << std::endl;
    return 0;
}