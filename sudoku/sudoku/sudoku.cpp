#include <sstream>
#include <vector>

#include "sudoku.hpp"
#include "timer.hpp"
#include "connectivityinfo.cpp"

bool Possible::is_on(unsigned int i) const { 
   return _b[i-1]; 
}

unsigned int Possible::count() const { 
   return _bc; 
}

void Possible::eliminate(unsigned int i) { 
   _b[i-1] = false;
   _bc--;
}

unsigned int val_count = 0;
unsigned int Possible::val() const {
   val_count++;
   for(unsigned int i = 0; i < 9; ++i)
      if(_b[i] == true)
         return i+1;
   return 10;
}

std::string Possible::str(unsigned int width) const {
   std::string s(width, ' ');
   unsigned int k = 0;
   for (unsigned int i = 1; i <= 9; i++) {
      if (is_on(i)) s[k++] = '0' + i;
   }
   return s;
}

bool Sudoku::is_solved() const {
   for (unsigned int k = 0; k < _cells.size(); k++) {
      if (_cells[k].count() != 1) {
         return false;
      }
   }
   return true;
}

void Sudoku::write(std::ostream& o) const {
   unsigned int width = 1;
   for (unsigned int k = 0; k < _cells.size(); k++) {
      width = std::max(width, 1 + _cells[k].count());
   }
   const std::string sep(3 * width, '-');
   for (unsigned int i = 0; i < 9; i++) {
      if (i == 3 || i == 6) {
         o << sep << "+-" << sep << "+" << sep << std::endl;
      }
      for (unsigned int j = 0; j < 9; j++) {
         if (j == 3 || j == 6) o << "| ";
         o << _cells[i*9 + j].str(width);
      }
      o << std::endl;
   }
}

//void Sudoku::init() {
//   for (unsigned int i = 0; i < 9; i++) {
//      for (unsigned int j = 0; j < 9; j++) {
//         const unsigned int k = i*9 + j;
//         const unsigned int x[3] = {i, 9 + j, 18 + (i/3)*3 + j/3};
//         for (unsigned int g = 0; g < 3; g++) {
//            _group[x[g]].push_back(k);
//            _groups_of[k].push_back(x[g]);
//         }
//      }
//   }
//   for (unsigned int k = 0; k < _neighbors.size(); k++) {
//      for (unsigned int x = 0; x < _groups_of[k].size(); x++) {
//         for (unsigned int j = 0; j < 9; j++) {
//            unsigned int k2 = _group[_groups_of[k][x]][j];
//            if (k2 != k) _neighbors[k].push_back(k2);
//         }
//      }
//   }
//}

void Sudoku::unsafe_assign(unsigned int k, unsigned int val) {
   for (unsigned int i = 1; i <= 9; i++) {
      if (i != val) 
         unsafe_eliminate(k, i);
   }
}

void Sudoku::unsafe_eliminate(unsigned int k, unsigned int val) {
   if (!_cells[k].is_on(val))
      return;

   _cells[k].eliminate(val);
   const unsigned int N = _cells[k].count();
   if (N == 1) {
      const unsigned int v = _cells[k].val();
      for (unsigned int i = 0; i < _neighbors[k].size(); i++)
         unsafe_eliminate(_neighbors[k][i], v);
   }
   for (unsigned int i = 0; i < _groups_of[k].size(); i++) {
      const unsigned int x = _groups_of[k][i];
      unsigned int n = 0, ks;
      for (unsigned int j = 0; j < 9; j++) {
         const unsigned int p = _group[x][j];
         if (_cells[p].is_on(val)) {
            n++, ks = p;
         }
      }
      if (n == 1)
         unsafe_assign(ks, val);
   }
   return;
}

bool Sudoku::assign(unsigned int k, unsigned int val) {
   for (unsigned int i = 1; i <= 9; i++) {
      if (i != val) {
         if (!eliminate(k, i))
            return false;
      }
   }
   return true;
}

bool Sudoku::eliminate(unsigned int k, unsigned int val) {
   if (!_cells[k].is_on(val)) {
      return true;
   }
   _cells[k].eliminate(val);
   const unsigned int N = _cells[k].count();
   if (N == 0) {
      return false;
   } else if (N == 1) {
      const unsigned int v = _cells[k].val();
      for (unsigned int i = 0; i < _neighbors[k].size(); i++) {
         if (!eliminate(_neighbors[k][i], v)) 
            return false;
      }
   }
   for (unsigned int i = 0; i < _groups_of[k].size(); i++) {
      const unsigned int x = _groups_of[k][i];
      unsigned int n = 0, ks;
      for (unsigned int j = 0; j < 9; j++) {
         const unsigned int p = _group[x][j];
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

unsigned int Sudoku::least_count() const {
   unsigned int k = 10, min = 10;
   for(unsigned int i = 0; i < _cells.size(); i++) {
      const unsigned int m = _cells[i].count();
      if(m == 2)
         return i;
      if (m > 1 && (k == 10 || m < min)) {
         min = m, k = i;
      }
   }
   return k;
}

Sudoku::Sudoku(std::string s) : _cells{} {
   unsigned int k = 0;
   for (unsigned int i = 0; i < s.size(); i++) {
      if (s[i] >= '1' && s[i] <= '9') {
         unsafe_assign(k, s[i] - '0');
         k++;
      } else if (s[i] == '0' || s[i] == '.') {
         k++;
      }
   }
}

void Sudoku::transform() const {
   Chrono t;
   t.reset();
   std::array<std::array<std::vector<unsigned int>, 9>, 9> s;
   std::array<std::array<unsigned int, 9>, 9> x;

   for(unsigned int i = 0; i < _cells.size(); ++i) {
      for(unsigned int j = 1; j <= 9; ++j) {
         if(_cells[i].is_on(j))
            s[i/9][i%9].push_back(j);
      }
      if(s[i/9][i%9].size() == 1)
         x[i/9][i%9] = s[i/9][i%9][0];
   }
   std::cout << t.split() << std::endl;
}

unsigned int Sudoku::solve(Sudoku S, unsigned int idx, unsigned int val) {
   if( val != 0 ) {
      unsigned int r = S.assign(idx, val);
      if( !r )
         return 0;
   }

   if( S.is_solved() )
      return 1;

   unsigned int solved = 0;
   const unsigned int k = S.least_count();
   for(unsigned int i = 1; i <= 9 && !solved; ++i) {
      if( S.possible(k).is_on(i) )
         solved = solve(S, k, i);
   }
   return solved;
}