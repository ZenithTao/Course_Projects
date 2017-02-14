
#include <iostream>
#include <string.h>

#include "lexanal.h"

using namespace std;

void printToken( int code, char *token );

int main(int argc, char ** argv)
{
   char inp[10000];
   char token[100];
   int  token_code;
   int  n;
   char *p;

   cout << "cs544SQL >> ";

   while (1)
   {
      n = read_command( inp );          // Read a command

      p = inp;				// Start parsing at p[0]

      while ( p != inp + n )		// p did not reach end of line
      {
         while ( (p != inp + n) && isspace(*p) )
	    p++;			// Skip space

	 p = lex_anal( p, token, &token_code );   // Get next token

         printToken( token_code, token );	// Print and check token
      }
   }
}
