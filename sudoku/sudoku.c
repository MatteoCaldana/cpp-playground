#include <stdio.h>
#include "timer.hpp"

typedef struct sk { int s[9][9]; } Grid;

Grid load_grid(const char *filename) {
  Grid s;
  FILE *fp = fopen(filename, "r");
  if( fp == NULL ) {
      printf("\n\n Grid NOT found \n\n" );
      fclose(fp);
  }
  for(int r = 0; r < 9; r++ )     
    for(int c = 0; c < 9; c++ )
      fscanf( fp, "%d", &s.s[r][c] ); 
  fclose(fp);
  return s;
}

void print(Grid s) {
  printf("\n\n");
  for(int r = 0; r < 9; ++r) {
    printf(" ");
    for(int c=0; c < 9; ++c )
      printf(" %c", s.s[r][c] ? '0'+s.s[r][c] : '-' );
    printf("\n");
  }
  printf("\n");
} 

/**************************************************************************/

int is_in_row(Grid s, int row, int val) {
  for(int i = 0; i < 9; ++i)
    if( s.s[row][i] == val ) return 1;
  return 0;
}

int is_in_col(Grid s, int col, int val) {
  for(int i = 0; i < 9; i++)
    if( s.s[i][col] == val ) return 1;
  return 0;
}

int is_in_qud(Grid s, int row, int col, int val) {
  int rq = row / 3;
  int cq = col / 3;
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      if( s.s[3*rq + i][3*cq + j] == val ) return 1;
  return 0;
}

int licit(Grid s, int row, int col, int val) {
  return ! is_in_row(s, row, val)
      && ! is_in_col(s, col, val)
      && ! is_in_qud(s, row, col, val);
}

int consistent( Grid s ) {
  for(int r = 0; r < 9; ++r) {
    for(int c = 0; c < 9; ++c ) {  
      int temp = s.s[r][c];
      if( temp ) {
        s.s[r][c] = 0;
        if( ! licit(s, r, c, temp) ) 
            return 0;
        s.s[r][c] = temp;
      }
    }
  }
  return 1;
}

/**************************************************************************/

int solve(Grid s, int r, int c) {
  int solved = 0;
  if( r == 8 && c == 8 )
    if( s.s[r][c] ) {
      //printf("\n\n   Grid RISOLTO :\n");  
      //print(s);
      return 1;
    }

  int nxt_row = r, nxt_col = c;
  if( c == 8 ) {
    if( r != 8 ) { nxt_col=0; nxt_row++; }   
  }
  else nxt_col++; 
  
  if( s.s[r][c] )
    return solve( s, nxt_row, nxt_col );

  int val = 1;
  while( val < 10 && ! solved ) {
    if( licit(s, r, c, val) ) {
      s.s[r][c] = val;
      solved = solve(s, nxt_row, nxt_col);
    }
    val++;
  }
  return solved;
}

/**************************************************************************/

int main( int argc, char *argv[]  ) {
  Grid s = load_grid("sudoku1.txt");
  printf("\n\n   Grid CARICATO :\n");  
  print(s);
  
  if( ! consistent(s) )
    printf("\n\n   Grid ERRATO, NON RISOLVIBILE :\n");  
  
  Chrono t;
  t.reset();
  if( solve(s, 0, 0) ) {
    auto elapsed_time = t.split();
    printf("\n\n   All'S WELL THAT ENDS WELL in %.4fs\n", elapsed_time / 1000000.0);
  }
  else
    printf("\n\n   Grid NON RISOLUBILE, MANAGG' \n");

  return 0;
}


