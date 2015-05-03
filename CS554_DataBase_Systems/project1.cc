// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// token_record TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <locale>
#include "lexanal.h"

using namespace std;

int read_command (char *input) {
	int i, pos;
	string res, current_string;
	
	while(1){
		i = 0;
		getline (cin,current_string);
  	  	
		while(current_string[i] == ' ' || current_string[i] == '\t') {	 
			i++;
		}
		if (i != current_string.size())	{
			//check go
			int check = 0;
			int pos1, pos2, pos3, pos4;
			pos1 = current_string.find("go",0);	
			pos2 = current_string.find("gO",0);
			pos3 = current_string.find("Go",0);
			pos4 = current_string.find("GO",0);
			if (pos1 == i || pos2 == i || pos3 == i || pos4 == i) {
				i += 2;
				while (current_string[i] == ' ' || current_string[i] == '\t') {
					i++;
				}
				if (i == current_string.size()) {
					break;
				}
			}
			res += current_string + ' ';
		}
	}
	// remove the last ' '
	strcpy (input, res.substr(0, res.size()-1).c_str());	
	
	// deal with only "go" case
	return (res.size() ? res.size()-1 : 0);	
}



char *peek(char *current_pos, char *token, int *token_type) {
	int ccc = 0;
	locale loc;
	char *postion, *postion_t;
	char *token_record;
	int test_float = 0;
	token_record = token;
	
	char char_ori[100];
	
	// start with the first character	
	if (isalpha(*current_pos)) {
		char_ori[0] = *current_pos;
		*token_record++ = toupper(*current_pos++, loc);
		ccc++;
		
		int i = 1;
		while (isalpha(*current_pos) || isdigit(*current_pos) || *current_pos == '_') {
			char_ori[i++] = *current_pos;
			*token_record++ = toupper(*current_pos++, loc);
			ccc++;
		}
		
		*token_record = '\0';
		char_ori[i] = '\0';
		
		if (strcmp( token, "ALL" ) == 0 )
			*token_type = ALL;
		else if (strcmp( token, "AND" ) == 0 )
			*token_type = AND;
		else if (strcmp( token, "ANY" ) == 0 )
			*token_type = ANY;
		else if (strcmp( token, "AS" ) == 0 )
			*token_type = AS;
		else if (strcmp( token, "AVG" ) == 0 )
			*token_type = AVG;
		else if (strcmp( token, "BETWEEN" ) == 0 )
			*token_type = BETWEEN;
		else if (strcmp( token, "BY" ) == 0 )
			*token_type = BY;
		else if (strcmp( token, "CHAR" ) == 0 )
			*token_type = CHAR;
		else if (strcmp( token, "CHECK" ) == 0 )
			*token_type = CHECK;
		else if (strcmp( token, "CLOSE" ) == 0 )
			*token_type = CLOSE;
		else if (strcmp( token, "COMMIT" ) == 0 )
			*token_type = COMMIT;
		else if (strcmp( token, "COUNT" ) == 0 )
			*token_type = COUNT;
		else if (strcmp( token, "CREATE" ) == 0 )
			*token_type = CREATE;
 		else if (strcmp( token, "DECIMAL" ) == 0 )
			*token_type = DECIMAL;
		else if (strcmp( token, "DELETE" ) == 0 )
			*token_type = DELETE;
		else if (strcmp( token, "DISTINCT" ) == 0 )
			*token_type = DISTINCT;
		else if (strcmp( token, "DOUBLE" ) == 0 )
			*token_type = DOUBLE;
		else if (strcmp( token, "DROP" ) == 0 )
			*token_type = DROP;
		else if (strcmp( token, "EXISTS" ) == 0 )
			*token_type = EXISTS;
		else if (strcmp( token, "FLOAT" ) == 0 )
			*token_type = FLOAT;
		else if (strcmp( token, "FROM" ) == 0 )
			*token_type = FROM;
		else if (strcmp( token, "GO" ) == 0 )
			*token_type = GO;
		else if (strcmp( token, "GROUP" ) == 0 )
			*token_type = GROUP;
		else if (strcmp( token, "HAVING" ) == 0 )
			*token_type = HAVING;
		else if (strcmp( token, "IN" ) == 0 )
			*token_type = IN;
		else if (strcmp( token, "INSERT" ) == 0 )
			*token_type = INSERT;
		else if (strcmp( token, "INT" ) == 0 )
			*token_type = INT;
		else if (strcmp( token, "INTO" ) == 0 )
			*token_type = INTO;
		else if (strcmp( token, "IS" ) == 0 )
			*token_type = IS;
		else if (strcmp( token, "LIKE" ) == 0 )
			*token_type = LIKE;
		else if (strcmp( token, "MAX" ) == 0 )
			*token_type = MAX;
		else if (strcmp( token, "MIN" ) == 0 )
			*token_type = MIN;
		else if (strcmp( token, "NOT" ) == 0 )
			*token_type = NOT;
		else if (strcmp( token, "NULL" ) == 0 )
			*token_type = NULL0;
		else if (strcmp( token, "NUMERIC" ) == 0 )
			*token_type = NUMERIC;
		else if (strcmp( token, "OF" ) == 0 )
			*token_type = OF;
		else if (strcmp( token, "ON" ) == 0 )
			*token_type = ON;
		else if (strcmp( token, "OR" ) == 0 )
			*token_type = OR;
		else if (strcmp( token, "ORDER" ) == 0 )
			*token_type = ORDER;
		else if (strcmp( token, "PRIMARY" ) == 0 )
			*token_type = PRIMARY;
		else if (strcmp( token, "REAL" ) == 0 )
			*token_type = REAL;
		else if (strcmp( token, "SCHEMA" ) == 0 )
			*token_type = SCHEMA;
		else if (strcmp( token, "SELECT" ) == 0 )
			*token_type = SELECT;
		else if (strcmp( token, "SET" ) == 0 )
			*token_type = SET;
		else if (strcmp( token, "SOME" ) == 0 )
			*token_type = SOME;
		else if (strcmp( token, "SUM" ) == 0 )
			*token_type = SUM;
		else if (strcmp( token, "TABLE" ) == 0 )
			*token_type = TABLE;
		else if (strcmp( token, "TO" ) == 0 )
			*token_type = TO;
		else if (strcmp( token, "UNION" ) == 0 )
			*token_type = UNION;
		else if (strcmp( token, "UNIQUE" ) == 0 )
			*token_type = UNIQUE;
		else if (strcmp( token, "UPDATE" ) == 0 )
			*token_type = UPDATE;
		else if (strcmp( token, "USER" ) == 0 )
			*token_type = USER;
		else if (strcmp( token, "VALUES" ) == 0 )
			*token_type = VALUES;
		else if (strcmp( token, "VIEW" ) == 0 )
			*token_type = VIEW;
		else if (strcmp( token, "WHERE" ) == 0 )
			*token_type = WHERE;
		else if (strcmp( token, "WITH" ) == 0 )
			*token_type = WITH;
		else {
			*token_type = IDENTIFIER;
			int i = 0;
			for (i = 0; i < strlen(char_ori); i++) {
				*token++ = char_ori[i];
			}
		}
	}
	else if (*current_pos == '.') { 
		*token_record++ = *current_pos++;
		ccc++;
		while (isdigit(*current_pos)) {
			*token_record++ = *current_pos++;
			ccc++;
		}
		*token_record = '\0';			
		*token_type = (strlen(token) == 1) ? '.' : FLOAT_NUMBER;
	}
	else if (isdigit(*current_pos) || *current_pos == '+' || *current_pos == '-') {
		int testdigit = 0, testplus = 0;
		if (isdigit(*current_pos)) testdigit = 1;
		if (*current_pos == '+')  testplus = 1;
		*token_record++ = *current_pos++;
		ccc++;
		if (!testdigit && *current_pos == ' ') {
			if (testplus) {
				*token_record = '\0';
				*token_type = 43;
			} else {
				*token_record = '\0';
				*token_type = 45;			
			}
		} else {
			while (isdigit(*current_pos) || *current_pos == '.') {
				if (*current_pos == '.') test_float = 1;
				*token_record++ = *current_pos++;
				ccc++;
				if (*current_pos == '.')  {
					test_float = 1;
					*token_record++ = *current_pos++;
					ccc++;
					while (isdigit(*current_pos)) {
						*token_record++ = *current_pos++;
						ccc++;
					}
					*token_record = '\0';			
					*token_type = FLOAT_NUMBER;
					break;
				}
			}
			*token_record = '\0';			
			*token_type = test_float ? FLOAT_NUMBER : INTEGER_NUMBER;
		}
	}
	else if (*current_pos == '\'') {
		*token_record++ = *current_pos++;
		ccc++;
		postion = current_pos;	
		postion_t = token_record;
		
		while(*current_pos != ' ' && *current_pos != '\0') {
			if (*current_pos == '\'') {
				postion = current_pos + 1;
				postion_t = token_record + 1;
				*token_type = STRING;
			}
			*token_record++ = *current_pos++;
			ccc++;
		}

		current_pos = postion;
		token_record = postion_t;
		*token_record = '\0';
		*token_type = (strlen(token) == 1) ? '\'' : STRING;
	}
	else if (*current_pos != '\0') {
		*token_record++ = *current_pos++;
		ccc++;
		
		if (*current_pos != '=')
			*token_type = *token; 
		else if (*token == '<') {
			*token_record++ = *current_pos++;
			ccc++;
			*token_type = LESSOREQ; 
		}
		else if (*token == '>') {
			*token_record++ = *current_pos++;
			ccc++;
			*token_type = GREATEROREQ; 
		}
		else if (*token == '!') {
			*token_record++ = *current_pos++;
			ccc++;
			*token_type = NOTEQ; 
		}
		else {
			*token_type = *token; 
		}
		*token_record = '\0';
	}
	else {
		*token_record = '\0';
		*token_type = *token;
	}

	// skip white inppute	
	while ( *current_pos == ' ' || *current_pos == '\t') {
		current_pos++;
		ccc++;
	}
	return current_pos-ccc;
} 


char *lex_anal(char *current_pos, char *token, int *token_type) {
	locale loc;
	char *postion, *postion_t;
	char *token_record;
	int test_float = 0;
	token_record = token;
	
	char char_ori[100];
	
	// start with the first character	
	if (isalpha(*current_pos)) {
		char_ori[0] = *current_pos;
		*token_record++ = toupper(*current_pos++, loc);
		
		int i = 1;
		while (isalpha(*current_pos) || isdigit(*current_pos) || *current_pos == '_') {
			char_ori[i++] = *current_pos;
			*token_record++ = toupper(*current_pos++, loc);
		}
		
		*token_record = '\0';
		char_ori[i] = '\0';
		
		if (strcmp( token, "ALL" ) == 0 )
			*token_type = ALL;
		else if (strcmp( token, "AND" ) == 0 )
			*token_type = AND;
		else if (strcmp( token, "ANY" ) == 0 )
			*token_type = ANY;
		else if (strcmp( token, "AS" ) == 0 )
			*token_type = AS;
		else if (strcmp( token, "AVG" ) == 0 )
			*token_type = AVG;
		else if (strcmp( token, "BETWEEN" ) == 0 )
			*token_type = BETWEEN;
		else if (strcmp( token, "BY" ) == 0 )
			*token_type = BY;
		else if (strcmp( token, "CHAR" ) == 0 )
			*token_type = CHAR;
		else if (strcmp( token, "CHECK" ) == 0 )
			*token_type = CHECK;
		else if (strcmp( token, "CLOSE" ) == 0 )
			*token_type = CLOSE;
		else if (strcmp( token, "COMMIT" ) == 0 )
			*token_type = COMMIT;
		else if (strcmp( token, "COUNT" ) == 0 )
			*token_type = COUNT;
		else if (strcmp( token, "CREATE" ) == 0 )
			*token_type = CREATE;
 		else if (strcmp( token, "DECIMAL" ) == 0 )
			*token_type = DECIMAL;
		else if (strcmp( token, "DELETE" ) == 0 )
			*token_type = DELETE;
		else if (strcmp( token, "DISTINCT" ) == 0 )
			*token_type = DISTINCT;
		else if (strcmp( token, "DOUBLE" ) == 0 )
			*token_type = DOUBLE;
		else if (strcmp( token, "DROP" ) == 0 )
			*token_type = DROP;
		else if (strcmp( token, "EXISTS" ) == 0 )
			*token_type = EXISTS;
		else if (strcmp( token, "FLOAT" ) == 0 )
			*token_type = FLOAT;
		else if (strcmp( token, "FROM" ) == 0 )
			*token_type = FROM;
		else if (strcmp( token, "GO" ) == 0 )
			*token_type = GO;
		else if (strcmp( token, "GROUP" ) == 0 )
			*token_type = GROUP;
		else if (strcmp( token, "HAVING" ) == 0 )
			*token_type = HAVING;
		else if (strcmp( token, "IN" ) == 0 )
			*token_type = IN;
		else if (strcmp( token, "INSERT" ) == 0 )
			*token_type = INSERT;
		else if (strcmp( token, "INT" ) == 0 )
			*token_type = INT;
		else if (strcmp( token, "INTO" ) == 0 )
			*token_type = INTO;
		else if (strcmp( token, "IS" ) == 0 )
			*token_type = IS;
		else if (strcmp( token, "LIKE" ) == 0 )
			*token_type = LIKE;
		else if (strcmp( token, "MAX" ) == 0 )
			*token_type = MAX;
		else if (strcmp( token, "MIN" ) == 0 )
			*token_type = MIN;
		else if (strcmp( token, "NOT" ) == 0 )
			*token_type = NOT;
		else if (strcmp( token, "NULL" ) == 0 )
			*token_type = NULL0;
		else if (strcmp( token, "NUMERIC" ) == 0 )
			*token_type = NUMERIC;
		else if (strcmp( token, "OF" ) == 0 )
			*token_type = OF;
		else if (strcmp( token, "ON" ) == 0 )
			*token_type = ON;
		else if (strcmp( token, "OR" ) == 0 )
			*token_type = OR;
		else if (strcmp( token, "ORDER" ) == 0 )
			*token_type = ORDER;
		else if (strcmp( token, "PRIMARY" ) == 0 )
			*token_type = PRIMARY;
		else if (strcmp( token, "REAL" ) == 0 )
			*token_type = REAL;
		else if (strcmp( token, "SCHEMA" ) == 0 )
			*token_type = SCHEMA;
		else if (strcmp( token, "SELECT" ) == 0 )
			*token_type = SELECT;
		else if (strcmp( token, "SET" ) == 0 )
			*token_type = SET;
		else if (strcmp( token, "SOME" ) == 0 )
			*token_type = SOME;
		else if (strcmp( token, "SUM" ) == 0 )
			*token_type = SUM;
		else if (strcmp( token, "TABLE" ) == 0 )
			*token_type = TABLE;
		else if (strcmp( token, "TO" ) == 0 )
			*token_type = TO;
		else if (strcmp( token, "UNION" ) == 0 )
			*token_type = UNION;
		else if (strcmp( token, "UNIQUE" ) == 0 )
			*token_type = UNIQUE;
		else if (strcmp( token, "UPDATE" ) == 0 )
			*token_type = UPDATE;
		else if (strcmp( token, "USER" ) == 0 )
			*token_type = USER;
		else if (strcmp( token, "VALUES" ) == 0 )
			*token_type = VALUES;
		else if (strcmp( token, "VIEW" ) == 0 )
			*token_type = VIEW;
		else if (strcmp( token, "WHERE" ) == 0 )
			*token_type = WHERE;
		else if (strcmp( token, "WITH" ) == 0 )
			*token_type = WITH;
		else {
			*token_type = IDENTIFIER;
			int i = 0;
			for (i = 0; i < strlen(char_ori); i++) {
				*token++ = char_ori[i];
			}
		}
	}
	else if (*current_pos == '.') { 
		*token_record++ = *current_pos++;
		while (isdigit(*current_pos)) 
			*token_record++ = *current_pos++;
		*token_record = '\0';			
		*token_type = (strlen(token) == 1) ? '.' : FLOAT_NUMBER;
	}
	else if (isdigit(*current_pos) || *current_pos == '+' || *current_pos == '-') {
		int testdigit = 0, testplus = 0;
		if (isdigit(*current_pos)) testdigit = 1;
		if (*current_pos == '+')  testplus = 1;
		*token_record++ = *current_pos++;
		
		if (!testdigit && *current_pos == ' ') {
			if (testplus) {
				*token_record = '\0';
				*token_type = 43;
			} else {
				*token_record = '\0';
				*token_type = 45;			
			}
		} else {
			while (isdigit(*current_pos) || *current_pos == '.') {
				if (*current_pos == '.') test_float = 1;
				*token_record++ = *current_pos++;
				if (*current_pos == '.')  {
					test_float = 1;
					*token_record++ = *current_pos++;
					while (isdigit(*current_pos)) 
						*token_record++ = *current_pos++;
					*token_record = '\0';			
					*token_type = FLOAT_NUMBER;
					break;
				}
			}
			*token_record = '\0';			
			*token_type = test_float ? FLOAT_NUMBER : INTEGER_NUMBER;
		}
	}
	else if (*current_pos == '\'') {
		*token_record++ = *current_pos++;
		postion = current_pos;	
		postion_t = token_record;
		
		while(*current_pos != ' ' && *current_pos != '\0') {
			if (*current_pos == '\'') {
				postion = current_pos + 1;
				postion_t = token_record + 1;
				*token_type = STRING;
			}
			*token_record++ = *current_pos++;
		}

		current_pos = postion;
		token_record = postion_t;
		*token_record = '\0';
		*token_type = (strlen(token) == 1) ? '\'' : STRING;
	}
	else if (*current_pos != '\0') {
		*token_record++ = *current_pos++;
		
		if (*current_pos != '=')
			*token_type = *token; 
		else if (*token == '<') {
			*token_record++ = *current_pos++;
			*token_type = LESSOREQ; 
		}
		else if (*token == '>') {
			*token_record++ = *current_pos++;
			*token_type = GREATEROREQ; 
		}
		else if (*token == '!') {
			*token_record++ = *current_pos++;
			*token_type = NOTEQ; 
		}
		else {
			*token_type = *token; 
		}
		*token_record = '\0';
	}
	else {
		*token_record = '\0';
		*token_type = *token;
	}

	// skip white inppute	
	while ( *current_pos == ' ' || *current_pos == '\t') {
		current_pos++;
	}
	
	return current_pos;
} 