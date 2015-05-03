
#include <iostream>
#include <string.h>
#include "lexanal.h"

using namespace std;

void printToken( int code, char *token )
{
   cout << "Token code = " << code << ", token = '" << token << "'" << endl;

   if ( code < 128 && code == *token && *(token+1) == '\0' )
   {
      return;
   }

   // Cannot check the categories, but print the fact only

   if ( code == IDENTIFIER )
   {
      cout << "IDENTIFIER" << endl;
      return;
   }

   if ( code == INTEGER_NUMBER)
   {
      cout << "INTEGER_NUMBER" << endl;
      return;
   }

   if ( code == FLOAT_NUMBER)
   {
      cout << "FLOAT_NUMBER" << endl;
      return;
   }

   if ( code == STRING )
   {
      cout << "STRING" << endl;
      return;
   }
	

   switch ( code )
   {
      case LESSOREQ: 
	if (strcmp( token, "<=" ) == 0 )
	   return;

      case GREATEROREQ: 
	if (strcmp( token, ">=" ) == 0 )
	   return;

      case NOTEQ: 
	if (strcmp( token, "!=" ) == 0 )
	   return;

      case ALL: 
	if (strcmp( token, "ALL" ) == 0 )
	   return;
      case AND: 
	if (strcmp( token, "AND" ) == 0 )
	   return;
      case ANY: 
	if (strcmp( token, "ANY" ) == 0 )
	   return;
      case AS: 
	if (strcmp( token, "AS" ) == 0 )
	   return;
      case AVG: 
	if (strcmp( token, "AVG" ) == 0 )
	   return;
      case BETWEEN: 
	if (strcmp( token, "BETWEEN" ) == 0 )
	   return;
      case BY: 
	if (strcmp( token, "BY" ) == 0 )
	   return;
      case CHAR: 
	if (strcmp( token, "CHAR" ) == 0 )
	   return;
      case CHECK: 
	if (strcmp( token, "CHECK" ) == 0 )
	   return;
      case CLOSE: 
	if (strcmp( token, "CLOSE" ) == 0 )
	   return;
      case COMMIT: 
	if (strcmp( token, "COMMIT" ) == 0 )
	   return;
      case COUNT: 
	if (strcmp( token, "COUNT" ) == 0 )
	   return;
      case CREATE: 
	if (strcmp( token, "CREATE" ) == 0 )
	   return;
      case DECIMAL: 
	if (strcmp( token, "DECIMAL" ) == 0 )
	   return;
      case DELETE: 
	if (strcmp( token, "DELETE" ) == 0 )
	   return;
      case DISTINCT: 
	if (strcmp( token, "DISTINCT" ) == 0 )
	   return;
      case DOUBLE: 
	if (strcmp( token, "DOUBLE" ) == 0 )
	   return;
      case DROP: 
	if (strcmp( token, "DROP" ) == 0 )
	   return;
      case EXISTS: 
	if (strcmp( token, "EXISTS" ) == 0 )
	   return;
      case FLOAT: 
	if (strcmp( token, "FLOAT" ) == 0 )
	   return;
      case FROM: 
	if (strcmp( token, "FROM" ) == 0 )
	   return;
      case GO: 
	if (strcmp( token, "GO" ) == 0 )
	   return;
      case GROUP: 
	if (strcmp( token, "GROUP" ) == 0 )
	   return;
      case HAVING: 
	if (strcmp( token, "HAVING" ) == 0 )
	   return;
      case IN: 
	if (strcmp( token, "IN" ) == 0 )
	   return;
      case INSERT: 
	if (strcmp( token, "INSERT" ) == 0 )
	   return;
      case INT: 
	if (strcmp( token, "INT" ) == 0 )
	   return;
      case INTO: 
	if (strcmp( token, "INTO" ) == 0 )
	   return;
      case IS: 
	if (strcmp( token, "IS" ) == 0 )
	   return;
      case LIKE: 
	if (strcmp( token, "LIKE" ) == 0 )
	   return;
      case MAX: 
	if (strcmp( token, "MAX" ) == 0 )
	   return;
      case MIN: 
	if (strcmp( token, "MIN" ) == 0 )
	   return;
      case NOT: 
	if (strcmp( token, "NOT" ) == 0 )
	   return;
      case NULL0: 
	if (strcmp( token, "NULL" ) == 0 )
	   return;
      case NUMERIC: 
	if (strcmp( token, "NUMERIC" ) == 0 )
	   return;
      case OF: 
	if (strcmp( token, "OF" ) == 0 )
	   return;
      case ON: 
	if (strcmp( token, "ON" ) == 0 )
	   return;
      case OR: 
	if (strcmp( token, "OR" ) == 0 )
	   return;
      case ORDER: 
	if (strcmp( token, "ORDER" ) == 0 )
	   return;
      case PRIMARY: 
	if (strcmp( token, "PRIMARY" ) == 0 )
	   return;
      case REAL: 
	if (strcmp( token, "REAL" ) == 0 )
	   return;
      case SCHEMA: 
	if (strcmp( token, "SCHEMA" ) == 0 )
	   return;
      case SELECT: 
	if (strcmp( token, "SELECT" ) == 0 )
	   return;
      case SET: 
	if (strcmp( token, "SET" ) == 0 )
	   return;
      case SOME: 
	if (strcmp( token, "SOME" ) == 0 )
	   return;
      case SUM: 
	if (strcmp( token, "SUM" ) == 0 )
	   return;
      case TABLE: 
	if (strcmp( token, "TABLE" ) == 0 )
	   return;
      case TO: 
	if (strcmp( token, "TO" ) == 0 )
	   return;
      case UNION: 
	if (strcmp( token, "UNION" ) == 0 )
	   return;
      case UNIQUE: 
	if (strcmp( token, "UNIQUE" ) == 0 )
	   return;
      case UPDATE: 
	if (strcmp( token, "UPDATE" ) == 0 )
	   return;
      case USER: 
	if (strcmp( token, "USER" ) == 0 )
	   return;
      case VALUES: 
	if (strcmp( token, "VALUES" ) == 0 )
	   return;
      case VIEW: 
	if (strcmp( token, "VIEW" ) == 0 )
	   return;
      case WHERE: 
	if (strcmp( token, "WHERE" ) == 0 )
	   return;
      case WITH: 
	if (strcmp( token, "WITH" ) == 0 )
	   return;
      default: 
	cout << "Illegal token code: " << code << ", token was: '"
		 << token << "'";
	return;
   }
}
