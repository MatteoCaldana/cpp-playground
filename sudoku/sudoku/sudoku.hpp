#include <array>
#include <iostream>
#include <string>

class Possible {
   std::array<bool,9> _b;
   unsigned int _bc;
public:
   Possible() : _b{true,true,true,true,true,true,true,true,true}, _bc(9) {}
   inline bool is_on(unsigned int i) const;
   inline unsigned int count() const;
   inline void eliminate(unsigned int i);
   inline unsigned int val() const;
   std::string str(unsigned int wth) const;
};

class Sudoku {
private:
   std::array<Possible, 81> _cells;
   void unsafe_eliminate(unsigned int k, unsigned int val);
   bool eliminate(unsigned int k, unsigned int val);

public:
   Sudoku(const std::string s);
   //static void init();

   Possible possible(unsigned int k) const { return _cells[k]; }
   bool is_solved() const;
   bool assign(unsigned int k, unsigned int val);
   void unsafe_assign(unsigned int k, unsigned int val);
   unsigned int least_count() const;
   void write(std::ostream& o) const;
   void transform() const;

   static unsigned int solve(Sudoku S, unsigned int idx = 0, unsigned int val = 0);
};