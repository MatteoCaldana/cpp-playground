#include <stdio.h>

int checkInDirezione(char **m, char p[], int i, int j, int sv, int so, int r, int c) {
  while ( i>=0 && j>=0 && i<r && j<c && *p!='\0' ) {     // Cicla finché o supera i bordi o ha conrollato tutta p
    if ( *(p++) != m[i][j] ) return  0;                  // se il k-esimo è diverso -> la parola non c'è in questa direzione
    i += sv;
    j += so;     // altrimenti si muove lungo la direzione data
  }
  return *p == '\0';
}

void markInDirezione(char **n, int len, int i, int j, int sv, int so ) {
  while ( len-- > 0 ) {
    n[i][j] = '~';         // marca con un carattere riservato la matrice ausiliaria n
    i += sv;
    j += so;               // si muove lungo la direzione data
  }
}

int cercaDaQui( char **m, char **n, char p[], int i, int j, int r, int c ) {
  int so, sv;
  for( so = -1; so <= 1; so++ )
    for( sv = -1; sv <= 1; sv++ )
      if( ( so || sv ) &&  checkInDirezione(m,p,i,j,sv,so,r,c) ) { //almeno uno degli spostamenti non nullo
        markInDirezione(n,strlen(p),i,j,sv,so);
        return 1;
      }
  return 0;
}

int main() {
  int cols, rows, maxdim, i, j, trovata;
  char *parola, **m, **notes;
  FILE *fp;

  fp = fopen("schema1.txt", "r");             // apertura del file
  fscanf(fp, "%d\n%d\n\n", &rows, &cols);    // leggo numero righe e numero colonne
  maxdim = (rows > cols) ? rows : cols;      // determino massima lunghezza di una parola nel puzzle

  m    =  (char **) malloc(rows*sizeof(char *));       // allocazione degli array multidimensionali
  notes = (char **) malloc(rows*sizeof(char *));
  for (i=0; i<rows; i++) {
    m[i]     = (char *) malloc(cols*sizeof(char));
    notes[i] = (char *) malloc(cols*sizeof(char));
  }
  parola = (char *) malloc((maxdim+2)*sizeof(char));   // stringa dinamica di maxdim caratteri + 1 per \n + 1 per il terminatore

  for( i=0; i<rows; i++ ) {                 // lettura del puzzle negli array multidimensionali
    for( j=0; j<cols; j++ )
      notes[i][j] = m[i][j] = fgetc(fp);
    fgetc(fp); // brucia il \n in fondo alla riga
  }
  fgetc(fp);   // brucia il \n dopo lo schema

  while( fgets(parola, maxdim+2, fp) != NULL ) {   // leggo parole, finché ce ne sono, di al più maxdim caratteri + 1 per il \n + 1 per il \0
    parola[strlen(parola)-1] = '\0';               // cancello il \n
    printf("\n  Parola letta : [%10s]", parola);
    trovata = 0;                                   // segno la parola come non ancora trovata
    for ( i=0; !trovata && i<rows ; i++ )          // per ogni casella del puzzle controlliamo se la parola inizia da (i,j)
      for ( j=0; !trovata && j<cols ; j++ )        // e se la trovo non la cerco oltre
        trovata = parola[0] == m[i][j] && cercaDaQui(m, notes, parola, i, j, rows, cols);
    printf(" %s trovata", trovata ? "   " : "NON");
  }
  fclose(fp);   // chiusura del file di input

  printf("\n\n SOLUZIONE DEL PUZZLE:\n\n    ");
  for ( i=0 ; i<rows ; i++ )         // stampiamo la "chiave" del puzzle
    for ( j=0 ; j<cols ; j++ )
      if ( notes[i][j] != '~' ) printf("%c", notes[i][j]);

  for (i=0; i<rows; i++) {           // liberazione della memoria allocata
    free(m[i]);
    free(notes[i]);
  }
  free(m);
  free(notes);

  printf("\n\n"); system("PAUSE");
  return 0;
}
