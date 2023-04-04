#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <sstream>

#include "timer.hpp"
#include "sudoku_data.cpp"

class Possible {
   std::array<bool,9> _b;
   uint _bc;
public:
   Possible() : _b{true,true,true,true,true,true,true,true,true}, _bc(9) {}
   inline bool is_on(uint i) const;
   inline uint count() const;
   inline void eliminate(uint i);
   inline uint val() const;
   std::string str(uint wth) const;
};

bool Possible::is_on(uint i) const { 
   return _b[i-1]; 
}

uint Possible::count() const { 
   return _bc; 
}

void Possible::eliminate(uint i) { 
   _b[i-1] = false;
   _bc--;
}

uint Possible::val() const {
   for(uint i = 0; i < 9; ++i)
      if(_b[i] == true)
         return i+1;
   return -1;
}

std::string Possible::str(uint width) const {
   std::string s(width, ' ');
   uint k = 0;
   for (uint i = 1; i <= 9; i++) {
      if (is_on(i)) s[k++] = '0' + i;
   }
   return s;
} 

class Sudoku {
   static std::array<std::array<uint,9>,27>  _group;
   static std::array<std::array<uint,24>,81> _neighbors;
   static std::array<std::array<uint,3>,81>  _groups_of;

   std::array<Possible, 81> _cells;

   inline bool eliminate(uint k, uint val);
public:
   Sudoku(std::string s);
   static void init();

   Possible possible(uint k) const { return _cells[k]; }
   inline bool is_solved() const;
   inline bool assign(uint k, uint val);
   inline uint least_count() const;
   inline void write(std::ostream& o) const;
};

bool Sudoku::is_solved() const {
   for (uint k = 0; k < _cells.size(); k++) {
      if (_cells[k].count() != 1) {
         return false;
      }
   }
   return true;
}

void Sudoku::write(std::ostream& o) const {
   uint width = 1;
   for (uint k = 0; k < _cells.size(); k++) {
      width = std::max(width, 1 + _cells[k].count());
   }
   const std::string sep(3 * width, '-');
   for (uint i = 0; i < 9; i++) {
      if (i == 3 || i == 6) {
         o << sep << "+-" << sep << "+" << sep << std::endl;
      }
      for (uint j = 0; j < 9; j++) {
         if (j == 3 || j == 6) o << "| ";
         o << _cells[i*9 + j].str(width);
      }
      o << std::endl;
   }
}

//std::vector< std::vector<uint> > 
//Sudoku::_group(27), Sudoku::_neighbors(81), Sudoku::_groups_of(81);
//
//void Sudoku::init() {
//   for (uint i = 0; i < 9; i++) {
//      for (uint j = 0; j < 9; j++) {
//         const uint k = i*9 + j;
//         const uint x[3] = {i, 9 + j, 18 + (i/3)*3 + j/3};
//         for (uint g = 0; g < 3; g++) {
//            _group[x[g]].push_back(k);
//            _groups_of[k].push_back(x[g]);
//         }
//      }
//   }
//   for (uint k = 0; k < _neighbors.size(); k++) {
//      for (uint x = 0; x < _groups_of[k].size(); x++) {
//         for (uint j = 0; j < 9; j++) {
//            uint k2 = _group[_groups_of[k][x]][j];
//            if (k2 != k) _neighbors[k].push_back(k2);
//         }
//      }
//   }
//
//}

bool Sudoku::assign(uint k, uint val) {
   for (uint i = 1; i <= 9; i++) {
      if (i != val) {
         if (!eliminate(k, i))
            return false;
      }
   }
   return true;
}

bool Sudoku::eliminate(uint k, uint val) {
   if (!_cells[k].is_on(val)) {
      return true;
   }
   _cells[k].eliminate(val);
   const uint N = _cells[k].count();
   if (N == 0) {
      return false;
   } else if (N == 1) {
      const uint v = _cells[k].val();
      for (uint i = 0; i < _neighbors[k].size(); i++) {
         if (!eliminate(_neighbors[k][i], v)) return false;
      }
   }
   for (uint i = 0; i < _groups_of[k].size(); i++) {
      const uint x = _groups_of[k][i];
      uint n = 0, ks;
      for (uint j = 0; j < 9; j++) {
         const uint p = _group[x][j];
         if (_cells[p].is_on(val)) {
            n++, ks = p;
         }
      }
      if (n == 0) {
         return false;
      } else if (n == 1) {
         if (!assign(ks, val)) {
            return false;
         }
      }
   }
   return true;
}

uint Sudoku::least_count() const {
   uint k = 10, min = 10;
   for(uint i = 0; i < _cells.size(); i++) {
      const uint m = _cells[i].count();
      if(m == 2)
         return i;
      if (m > 1 && (k == 10 || m < min)) {
         min = m, k = i;
      }
   }
   return k;
}

Sudoku::Sudoku(std::string s) : _cells{} {
   uint k = 0;
   for (uint i = 0; i < s.size(); i++) {
      if (s[i] >= '1' && s[i] <= '9') {
         if (!assign(k, s[i] - '0'))
            return;
         k++;
      } else if (s[i] == '0' || s[i] == '.') {
         k++;
      }
   }
}

uint t_sa = 0;
uint solve(Sudoku S, uint idx = 0, uint val = 0) {
   Chrono t;
   if( val != 0 ) {
      t.reset();
      uint r = S.assign(idx, val);
      t_sa += t.split();
      if( !r )
         return 0;
   }

   if( S.is_solved() ) {
      //S.write(std::cout);
      return 1;
   }

   uint solved = 0;
   uint k = S.least_count();
   for(uint i = 1; i <= 9 && !solved; ++i) {
      auto tmp = S.possible(k);
      if( tmp.is_on(i) )
         solved = solve(S, k, i);
   }
   return solved;
}

int main() {
   Sudoku::init();

   std::ifstream infile("hardest.txt");
   std::vector<std::string> sudokus;
   std::string line;

   while(getline(infile, line))
      sudokus.push_back(line);

   Chrono t;
   uint tot_time = 0.0;

   for(const auto sudoku : sudokus) {
      t.reset();

      Sudoku S(sudoku);
      solve(S);

      auto elapsed_time = t.split();

      tot_time += elapsed_time;
      //S_solved->write(std::cout);

      //if(!S_solved)
      // std::cout << "ERROR!" << std::endl;
      //std::cout << std::endl;
   }
   std::cout << tot_time << " " << t_sa << std::endl;
}