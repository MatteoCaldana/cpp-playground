#include <stdio.h>

#define FILENAME "vuoto"  /* A :   B:    C:   */
#define BLABLA   0

typedef struct sk { short int s[9][9]; } Schema;

Schema loadschema( char * filename ) {
  Schema s;
  int r, c, i;
  FILE * fp = fopen(filename, "r");
  if( fp == NULL ) {
      printf("\n\n SCHEMA NON TROVATO \n\n" );
      fclose(fp);
  }
  for( r=0; r<9; r++ )     
    for( c=0; c<9; c++ )
      fscanf( fp, "%d", &s.s[r][c] ); 
  fclose(fp);        
  return s;
}

void stampa( Schema s ) {
  int R, C;
  printf("\n\n");
  for( R=0; R<9; R++ ) {
    printf(" ");
    for( C=0; C<9; C++ )
      printf(" %c", s.s[R][C] ? '0'+s.s[R][C] : '-' );
    printf("\n");
  }
  printf("\n");
} 

/**************************************************************************/

int isInRiga( Schema s, int R, int val ) {
  int i;
  for( i=0; i<9; i++ )
    if( s.s[R][i] == val ) return 1;
  return 0;
}

int isInColonna( Schema s, int C, int val ) {
  int i;
  for( i=0; i<9; i++ )
    if( s.s[i][C] == val ) return 1;
  return 0;
}

/* rq e cq hanno valori da 0,0 a 2,2 */
int isInQuadrante( Schema s, int rq, int cq, int val ) {
  int x, y;
  for( x=0; x<3; x++ )
    for( y=0; y<3; y++ )
      if( s.s[3*rq+x][3*cq+y] == val ) return 1;
  return 0;
}

int lecito( Schema s, int R, int C, int val ) { /* Dice se mettere val in R,C � lecito  */
  return ! isInRiga( s, R, val ) &&
         ! isInColonna( s, C, val ) &&
         ! isInQuadrante( s, R/3, C/3, val );
}

int autocontraddittorio( Schema s ) {
  int r, c, temp;
  for( r=0; r<9; r++ )
    for( c=0; c<9; c++ ) {  /* di ogni casella con un valore espresso */
      temp = s.s[r][c];     /* controllo se lo stesso valore � anche dove non potrebbe */ 
      if( temp ) {          /* per riusare altre funzioni, attuo un'astuzia */
        s.s[r][c] = 0;                            /* svuoto per un attimo la casella */
        if( ! lecito( s, r, c, temp ) ) return 1;  /* controllo se ripristinarla � lecito */
        s.s[r][c] = temp;                         /* e se s�, la ripristino e continuo */
      }
    }
  return 0;   /* Se non trova niente di illecito */
}

/**************************************************************************/

int tenta( Schema s, int r, int c  ) {
  int risolto = 0, val = 1, nxtR = r, nxtC = c;
  if( r == 8 && c == 8 )
    if( s.s[r][c] ) {              /* Se lo ha riempito, lo ha fatto giusto! */
      printf("\n\n   SCHEMA RISOLTO :\n");  
      stampa(s);
      return 1;                      /* Se ho finito, esulto */
    }

  if( c == 8 ) {                          /* Determino la "prossima" casella */
    if( r != 8 ) { nxtC=0; nxtR++; }   
  }
  else  nxtC++; 
  
  if( s.s[r][c] )                    /* Se la casella corrente � piena */
    return tenta( s, nxtR, nxtC );      /* passo subito alla successiva */

  while( val < 10 && ! risolto ) {     /* senn� tento con tutti i valori */
    if( lecito(s,r,c,val) ) {      /* MA TENTO SOLO SE val QUI E' UN VALORE LECITO!! */
      if(BLABLA) printf("\n\nPROVA!! in %d,%d proviamo il numero [%d] (lecito) \n", r, c, val);
      s.s[r][c] = val;
      risolto = tenta( s, nxtR, nxtC );
    }
    else
      if(BLABLA) printf("\n\n (in %d,%d non provo [%d] perch� non � lecito)\n", r, c, val);
    val++;
  }
  return risolto;
}

/**************************************************************************/

int main( int argc, char * argv[]  ) {
  Schema s = loadschema( "sudoku1.txt" );
  printf("\n\n   SCHEMA CARICATO :\n");  
  stampa(s);
  
  if( autocontraddittorio(s) )
    printf("\n\n   SCHEMA ERRATO, NON RISOLVIBILE :\n");  
  
  if( tenta( s, 0, 0 ) )
    printf("\n\n   All'S WELL THAT ENDS WELL \n");
  else
    printf("\n\n   SCHEMA NON RISOLUBILE, MANAGG' \n");
  return 0;
}


