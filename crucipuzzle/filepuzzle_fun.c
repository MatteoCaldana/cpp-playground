#include <stdio.h>

#define FILE_NAME "schema1.txt"

/*
 * Il programma legge un file le cui prime due righe contengono due interi,
 * rispettivamente il numero di righe e di colonne dello schema, la terza riga
 * è vuota, dalla quarta riga in poi ci sono le righe dello schema, su ciascuna
 * delle quali i caratteri sono consecutivi. Poi c'è un'altra linea bianca, e
 * infine la lista di parole da cercare nello schema, ognuna su una riga.
 *
 * Alloca la struttura Puzzle, che contiene due interi pari alle dimensioni
 * dello schema e due puntatori a matrici dinamiche di tali dimensioni.
 *
 * Carica lo schema in entrambe le matrici dinamiche. La prima (schema) resta
 * inalterata durante l'esecuzione, dall'altra (notes) vengono progressivamente
 * cancellate le lettere delle parole trovate (lette scandendo il file).
 *
 * Le lettere rimaste sul notes dopo tutte le cancellazioni formano la "chiave".
 *
 */

typedef struct {
    int rows, cols;
    char ** schema;
    char ** notes;
 } Puzzle;

int allocaPuzzle ( Puzzle * , FILE * );

int caricaPuzzle ( Puzzle, FILE * );

void stampaSchema( Puzzle );

void stampaChiave( Puzzle );

void deallocaPuzzle ( Puzzle );


/*
 * La funzione check cerca la parola p nello schema di s a partire dal carattere
 * in posizione i,j secondo la direzione indicata dagli incrementi sv ed so
 * (spostamento verticale e orizzontale), che possono valere 1, 0 o -1.
 * Se la parola p è presente nello schema di s, la funzione scrive nella
 * matrice "ausiliaria" di s (notes), nelle stesse posizioni dei caratteri che
 * formano la parola p, un carattere riservato ('~'), per marcare le posizioni.
 *
 */
void check(Puzzle s, char p[], int i, int j, int sv, int so) {

  int k = 0;
  int len = strlen(p);

  // Cicla finché o supera i bordi o ha controllato tutta p
  while ( i>=0 && j>=0 && i<s.rows && j<s.cols && k<len ) {
    if ( p[k++] != s.schema[i][j] )
      return; // se il k-esimo è diverso -> esce
    // altrimenti si muove di un passo lungo la direzione indicata da sv e so
    i += sv;
    j += so;
  }

  if ( k == len ) // se siamo usciti dal ciclo perché la parola è stata trovata
    while ( k > 0 ) { // segno una tilde in tutte le posizioni della parola
      i -= sv;
      j -= so;
      s.notes[i][j] = '~'; // lo "schema" resta invariato, tocco solo il "notes"
      k--;
    }
}



int main() {

  int maxdim, i, j;
  char *parola, *tmp;
  Puzzle pz;
  FILE *fp;

  // apertura del file
  fp = fopen(FILE_NAME, "r");
  if ( fp == NULL ) {
    printf("\n\n  ERRORE APERTURA FILE \n\n");
    system("PAUSE");
    return -1;
  }

  if ( ! allocaPuzzle( &pz, fp ) ) {
    printf("\n\n  ERRORE ALLOCAZIONE SCHEMA \n\n");
    system("PAUSE");
    return -2;
  }

  if ( ! caricaPuzzle( pz, fp ) ) {
    printf("\n\n  ERRORE LETTURA SCHEMA \n\n");
    system("PAUSE");
    return -3;
  }

  stampaSchema( pz );

  // determina la massima lunghezza di una parola nel puzzle
  if ( pz.rows > pz.cols )
    maxdim = pz.rows;
  else
    maxdim = pz.cols;

  // allocazione di un array di caratteri per contenere una parola
  // di maxdim caratteri + uno per \n + uno per il terminatore
  parola = (char *) malloc((maxdim+2)*sizeof(char));
  if (parola == NULL) {
    printf("Problema di allocazione memoria per la stringa di confronto\n");
    return -4;
  }

  // leggiamo parole, finché ce ne sono, di al più maxdim caratteri
  // + uno per il \n
  printf("\n Words to be found:\n");
  while( fgets(parola, maxdim+2, fp) != NULL ) {
    parola[strlen(parola)-1] = '\0'; // cancello il \n
    printf("\n   %s", parola);

    // per ogni casella (i,j) del puzzle controlliamo se
    // la parola inizia da (i,j) e continua lungo una direzione data
    for ( i = 0 ; i < pz.rows ; i++ )
      for ( j = 0 ; j < pz.cols ; j++ ) {
        // effettuiamo il controllo per tutte le 8 direzioni possibili
        check(pz, parola, i, j,  0,  1);
        check(pz, parola, i, j,  0, -1);
        check(pz, parola, i, j,  1,  0);
        check(pz, parola, i, j, -1,  0);
        check(pz, parola, i, j,  1,  1);
        check(pz, parola, i, j, -1, -1);
        check(pz, parola, i, j,  1, -1);
        check(pz, parola, i, j, -1,  1);
      }
  }

  // chiusura del file
  fclose(fp);

  printf("\n\n The solution has been computed. ");
  system("PAUSE");  /* Per Dev-C++ su sistema Windows */

  stampaChiave( pz );

  // libera la memoria dinamica allocata
  deallocaPuzzle( pz );

  system("PAUSE");  /* Per Dev-C++ su sistema Windows */
  return 0;

}

int allocaPuzzle (Puzzle *p, FILE *fp) {

  int i;

  // lettura numero righe e numero colonne
  fscanf(fp, "%d\n%d\n\n", &(p->rows), &(p->cols));

  // allocazione degli array multidimensionali
  p->schema = (char **) malloc(p->rows*sizeof(char *));
  p->notes  = (char **) malloc(p->rows*sizeof(char *));

  if ( p->schema == NULL || p->notes == NULL ) {
    printf("Problema di allocazione memoria\n");
    return 0;
  }

  for ( i = 0; i < p->rows; i++ ) {
    p->schema[i] = (char *) malloc(p->cols*sizeof(char));
    p->notes[i]  = (char *) malloc(p->cols*sizeof(char));
    if ( p->schema[i] == NULL || p->notes[i] == NULL) {
      printf("Problema di allocazione memoria\n");
      return 0;
    }
  }

  return 1;
}

int caricaPuzzle (Puzzle p, FILE *fp) {

  int i, j;
  char ch;

  // lettura del puzzle negli array multidimensionali
  for ( i = 0; i < p.rows; i++ ) {
    for ( j = 0; j < p.cols; j++ ) {
      fscanf( fp, "%c", &ch );
      p.notes[i][j] = p.schema[i][j] = ch;
    }
    ch = fgetc(fp); // brucia il \n in fondo alla riga
  }
  ch = fgetc(fp);   // brucia il \n dopo lo schema

  return 1;  // Al momento non considera nessun errore - si può "irrobustire"
}


void stampaSchema( Puzzle p ) {

  int i,j;

  printf("\n Schema:\n\n");

  // stampiamo lo schema
  for ( i = 0 ; i < p.rows ; i++ ) {
    for ( j = 0 ; j < p.cols ; j++ )
      printf("  %c", p.schema[i][j]);
    printf("\n\n");
  }
}

void stampaChiave( Puzzle p ) {

  int i,j;

  // stampiamo la "chiave" del puzzle
  printf("\n\n Solution :    ");
  for ( i = 0 ; i < p.rows ; i++ )
    for ( j = 0 ; j < p.cols ; j++ )
      if ( p.notes[i][j] != '~' )
        printf("%c", p.notes[i][j]);

  printf("\n\n\n");

}

void deallocaPuzzle( Puzzle p ) {

  int i;

  for ( i = 0; i < p.rows; i++ ) {
    free(p.schema[i]);
    free(p.notes[i]);
  }
  free(p.schema);
  free(p.notes);

}
