// Based on log-sort (http://mathcs.emory.edu/~cs524000/share/1126/log-sort.c)
// implement the Burrows–Wheeler transform algorithm
// Liang Tao

#include <stdio.h>              /* fprintf, stderr, putchar */
#include <string.h>

const char *TAPE;               /* read-only input, NUL terminated  */
int end_of_tape(int i) { return TAPE[i] == '\0'; } /* NUL? */

int end_char(int i) { char c = TAPE[i]; return c==',' || c=='\0'; }

int end_string(int i) { while (!end_char(i)) ++i; return i; }

int next_string(int i) { i=end_string(i); if (TAPE[i]==',') ++i; return i; }

void print_string(int i) { while (!end_char(i)) putchar(TAPE[i++]); }

// here I implement a new method to print out the last character of sorted 
// string arrays
void print_lastChar(int i) {
    while (!end_char(i)) {
        i++;
    }
    putchar(TAPE[i-1]);
}

int sorts_before(int i, int j)  /* compare strings starting at i and j */
{
  if (i==j) return 0;
  while (1) {
    int end_i = end_char(i), end_j = end_char(j);
    if (end_i && end_j) return (i < j); /* stable sort */
    if (end_i || end_j) return end_i; /* prefixes first */
    if (TAPE[i]!=TAPE[j]) return (TAPE[i] < TAPE[j]);
    ++i; ++j;
  }
}

/* rank(i) == number of strings which should appear before the string at i */
int rank(int i)
{
  int ret=0, j;
  for (j=0; !end_of_tape(j); j=next_string(j))
    if (sorts_before(j,i)) ++ret;
  return ret;
}

void sort()
{
  int i=0, r=0, j;
  while (1)                     /* for each r = 0, 1, ... */
  {
    for (j=0; !end_of_tape(j); j=next_string(j))
      if (rank(j)==r) break;    /* find string of rank r */
    // here we don't need to print out all the strings any more
    // instead, just print out the last character of all sorted
    // strings as BWT indicates
    print_lastChar(j);            /* and print it */
    ++r;
    i = next_string(i);
    if (end_of_tape(i)) break;  /* last r done? */
    //putchar(',');
  }
  putchar('\n');
}

// int logsort(int argc, const char **argv)
// {
//   if (argc!=2) {
//     fprintf(stderr, "%d: expected one argument, got %d\n", argv[0], argc-1);
//     return 1;
//   }
//   TAPE = argv[1];
//   sort();
//   return 0;
// }

// new version of main to implement the BWT algorithm
int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "%s: expected one argument, got %d\n", argv[0], argc-1);
    return 1;
  }
  
  // prepare for the transformation
  const char *in = argv[1];
  int len = strlen(in);
  char newin[len+3];
  newin[0] = '^';
  int i;
  
  for (int i = 1; i <= len; i++) {
    newin[i] = in[i-1];
  }
  
  newin[len+1] = '|';
  newin[len+2] = '\0';
  
  int n = strlen(newin);
  int j, k, m;
  
  char RES[100000];
  strcat(RES,newin);
  
  for (i = n - 1; i >= 1; i--) {
    strcat(RES, ",");
    char new[n+1];
    int s = 0;
    k = i;
    
    for (j = 1; j <= n - i; j++) {
    new[s++] = newin[k++];
    }

    int m = 0;
    for (j = i; j >= 1; j--) {
    new[s++] = newin[m++];
    }

    new[n] = '\0';
    strcat(RES, new);
  }
  
  TAPE = RES;
  
  sort();
  return 0;
}