/*
 *  Sudoku game
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXBUFFER 128

int **acquisisci(char* nomefile, int *dim_ptr);
int controlla(int **schema, int dim, int passo);
int trova(int **schema, int dim, int passo);

/********************************************************************
  Main program
********************************************************************/
int main(int argc, char* argv[])
{
   int **schema, dim, i, j, risultato;

   schema = acquisisci(argv[1], &dim);

   if (trova(schema, dim, 0)) {
      printf("Soluzione:\n");
      for (i=0; i<dim; i++) {
	     for (j=0; j<dim; j++) {
	        printf("%3d ", schema[i][j]);
	     }
	     printf("\n");
      }
   } else {
      printf("Nessuna soluzione trovata.\n");
   }

   for (i=0; i<dim; i++) {
      free(schema[i]);
   }
   free(schema);
   //system("pause");
   return risultato;
}

/********************************************************************
  Function to read the starting schema from file
********************************************************************/
int **acquisisci(char* nomefile, int *dim_ptr)
{
   int i, j, **schema, dimensione=0;
   char buf[MAXBUFFER];
   FILE *fp;

   fp = fopen(nomefile, "r");
   if (fp == NULL) {
      printf("Errore durante l'apertura del file\n");
      exit(1);
   }

   /* read once the input file to know the sudoku table size */
   while (fgets(buf, MAXBUFFER, fp) != NULL) {
      dimensione++;
   }
   fclose(fp);

   /* allocate necessary memory */
   schema = (int**)malloc((dimensione)*sizeof(int*));
   if (schema == NULL) {
      printf("Errore durante l'allocazione della memoria\n");
      exit(1);
   }

   for (i=0; i<dimensione; i++) {
      schema[i] = (int*)malloc(dimensione*sizeof(int));
      if (schema[i] == NULL) {
	     printf("Errore durante l'allocazione della memoria\n");
	     exit(1);
      }
   }

   /* read again the input file to save data */
   fp = fopen(nomefile, "r");
   if (fp == NULL) {
      printf("Errore durante l'apertura del file\n");
      exit(1);
   }

   for (i=0; i<dimensione; i++) {
      for (j=0; j<dimensione; j++) {
	     fscanf(fp, "%d", &schema[i][j]);
      }
   }
   fclose(fp);

   *dim_ptr = dimensione;
   return schema;
}

/********************************************************************
  Ricorsive function to find a solution
********************************************************************/
int trova(int **schema, int dim, int passo)
{
   int i, j, k;

   /* whene the recursive search must return */
   if (passo >= dim*dim) {
      return 1;
   }

   /* current box indices */
   i = passo / dim;
   j = passo % dim;
   if (schema[i][j] != 0) {
      /* box is already full: nothing to be done */
      return trova(schema, dim, passo+1);
   }

   /* i try all the possible value between 1 and dim */
   for (k=1; k<=dim; k++) {
      schema[i][j] = k;
      if (controlla(schema, dim, passo)) {
	     if (trova(schema, dim, passo+1)) {
	        return 1;
         }
      }
      schema[i][j] = 0;
   }

   return 0;
}

/********************************************************************
  Function to check if a partial solution is ok
********************************************************************/
int controlla(int **schema, int dim, int passo)
{
   int i, j, k, h, val, ib, jb, n=sqrt(dim);

   /* current box indices */
   i = passo / dim;
   j = passo % dim;
   val = schema[i][j];

   /* check the line */
   for (h=0; h<dim; h++)
      if ((schema[i][h]==val) && (h!=j))
	     return 0;

   /* check the row */
   for (k=0; k<dim; k++)
      if ((schema[k][j]==val) && (k!=i))
	     return 0;

   /* starting indices of the box to be checked */
   ib = (i/n) * n;
   jb = (j/n) * n;

   /* check the box */
   for (k=ib; k<ib+n; k++)
      for (h=jb; h<jb+n; h++)
	     if ((schema[k][h]==val) && (k!=i || h!=j))
	        return 0;

   return 1;
}
