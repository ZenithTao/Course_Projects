#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "ScanTable.h"


int main(int argc, char *argv[])
{
   int i, j;


   if ( argc != 2 )
   {
      cout << "Usage: " << argv[0] << " relname" << endl;
      exit(1);
   }

   cout << "Program for printing a mini database:" << endl << endl;

   /* ---
      Open database file
      --- */
   ScanTable *f = new ScanTable();

   if ( f->Open( argv[1] ) == 0 )
   {
      cout << "Relation '" << argv[1] << "' not found !" << endl;
      exit(1);
   }

   f->PrintRelationInfo();


   while ( f->GetNext() != 0 )
   {
      /* ------------------
         Print it...
         ------------------ */
      f->PrintTuple();
   }

   f->Close();

   printf("\n");
   printf("\n");


}

