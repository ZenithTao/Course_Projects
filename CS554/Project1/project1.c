#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexanal.h"

void uppercase(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		str[i] = toupper(str[i]);
	}
}

int read_command(char inp[10000]) {
 	int i = 0;
 	char str[1000];
 	int count = 0;
	while (1) {
		fgets(str, 1000, stdin);
		count++;
		str[strlen(str) - 1] = ' ';
		if (strcmp("go ", str) == 0 || strcmp("gO ", str) == 0 || strcmp("Go ", str) == 0 || strcmp("GO ", str) == 0) {
			inp[i-1] = '\0';
			return i - count + 1;
		} else {
			for (int j = 0; j < strlen(str); j++) {
				inp[i++] = str[j];
			}
		}
	}
}

char *lex_anal(char *current_pos, char *token, int *token_type) {
	int i = 0;
	int j = 0;
	memset(token, 0, sizeof(char));
	// while (current_pos[i] == ' ') {
	// 	i++;
	// }
	while (current_pos[i] != ' ') {
		token[j++] = current_pos[i++];
	}
	
	token[j] = '\0';
	uppercase(token);
	
	printf("%s\n", token);
	if (strcmp(",", token) == 0) {
		*token_type = ',';
	} else if (strcmp("(", token) == 0) {
		*token_type = '(';
	} else if (strcmp(")", token) == 0) {
		*token_type = ')';
	} else if (strcmp("[", token) == 0) {
		*token_type = '[';
	} else if (strcmp("]", token) == 0) {
		*token_type = ']';	
	} else if (strcmp(".", token) == 0) {
		*token_type = '.';
	} else if (strcmp(":", token) == 0) {
		*token_type = ':';	
	} else if (strcmp(";", token) == 0) {
		*token_type = ';';
	} else if (strcmp("*", token) == 0) {
		*token_type = '*';
	} else if (strcmp("+", token) == 0) {
		*token_type = '+';
	} else if (strcmp("-", token) == 0) {
		*token_type = '-';
	} else if (strcmp("/", token) == 0) {
		*token_type = '/';
	} else if (strcmp("<", token) == 0) {
		*token_type = '<';	
	} else if (strcmp(">", token) == 0) {
		*token_type = '>';
	} else if (strcmp("=", token) == 0) {
		*token_type = '=';
	} else if (strcmp("<=", token) == 0) {
		*token_type = LESSOREQ;
	} else if (strcmp(">=", token) == 0) {
		*token_type = GREATEROREQ;	
	} else if (strcmp("!=", token) == 0) {
		*token_type = NOTEQ;
	} else if (strcmp("ALL", token) == 0) {
		*token_type = ALL;
	} else if (strcmp("AND", token) == 0) {
		*token_type = AND;	
	} else if (strcmp("ANY", token) == 0) {
		*token_type = ANY;
	} else if (strcmp("AS", token) == 0) {
		*token_type = AS;	
	} else if (strcmp("AVG", token) == 0) {
		*token_type = AVG;		
	} else if (strcmp("BETWEEN", token) == 0) {
		*token_type = BETWEEN;	
	} else if (strcmp("BY", token) == 0) {
		*token_type = BY;			
	} else if (strcmp("CHAR", token) == 0) {
		*token_type = CHAR;			
	} else if (strcmp("CHECK", token) == 0) {
		*token_type = CHECK;			
	} else if (strcmp("CLOSE", token) == 0) {
		*token_type = CLOSE;			
	} else if (strcmp("COMMIT", token) == 0) {
		*token_type = COMMIT;	
	} else if (strcmp("COUNT", token) == 0) {
		*token_type = COUNT;			
	} else if (strcmp("CREATE", token) == 0) {
		*token_type = CREATE;	
	} else if (strcmp("DECIMAL", token) == 0) {
		*token_type = DECIMAL;			
	} else if (strcmp("DELETE", token) == 0) {
		*token_type = DELETE;	
	} else if (strcmp("DISTINCT", token) == 0) {
		*token_type = DISTINCT;			
	} else if (strcmp("DOUBLE", token) == 0) {
		*token_type = DOUBLE;	
	} else if (strcmp("DROP", token) == 0) {
		*token_type = DROP;			
	} else if (strcmp("EXISTS", token) == 0) {
		*token_type = EXISTS;			
	} else if (strcmp("FLOAT", token) == 0) {
		*token_type = FLOAT;			
	} else if (strcmp("FROM", token) == 0) {
		*token_type = FROM;			
	} else if (strcmp("GO", token) == 0) {
		*token_type = GO;			
	} else if (strcmp("GROUP", token) == 0) {
		*token_type = GROUP;
	} else if (strcmp("HAVING", token) == 0) {
		*token_type = HAVING;			
	} else if (strcmp("IN", token) == 0) {
		*token_type = IN;			
	} else if (strcmp("INSERT", token) == 0) {
		*token_type = INSERT;			
	} else if (strcmp("INT", token) == 0) {
		*token_type = INT;			
	} else if (strcmp("INTO", token) == 0) {
		*token_type = INTO;			
	} else if (strcmp("IS", token) == 0) {
		*token_type = IS;
	} else if (strcmp("LIKE", token) == 0) {
		*token_type = LIKE;			
	} else if (strcmp("MAX", token) == 0) {
		*token_type = MAX;			
	} else if (strcmp("MIN", token) == 0) {
		*token_type = MIN;			
	} else if (strcmp("NOT", token) == 0) {
		*token_type = NOT;			
	} else if (strcmp("NUMERIC", token) == 0) {
		*token_type = NUMERIC;					
	} else if (strcmp("OF", token) == 0) {
		*token_type = OF;			
	} else if (strcmp("ON", token) == 0) {
		*token_type = ON;			
	} else if (strcmp("OR", token) == 0) {
		*token_type = OR;			
	} else if (strcmp("ORDER", token) == 0) {
		*token_type = ORDER;			
	} else if (strcmp("PRIMARY", token) == 0) {
		*token_type = PRIMARY;			
	} else if (strcmp("REAL", token) == 0) {
		*token_type = REAL;				
	} else if (strcmp("SCHEMA", token) == 0) {
		*token_type = SCHEMA;				
	} else if (strcmp("SELECT", token) == 0) {
		*token_type = SELECT;				
	} else if (strcmp("SET", token) == 0) {
		*token_type = SET;	
	} else if (strcmp("SOME", token) == 0) {
		*token_type = SOME;	
	} else if (strcmp("SUM", token) == 0) {
		*token_type = SUM;	
	} else if (strcmp("TABLE", token) == 0) {
		*token_type = TABLE;	
	} else if (strcmp("TO", token) == 0) {
		*token_type = TO;	
	} else if (strcmp("UNION", token) == 0) {
		*token_type = UNION;	
	} else if (strcmp("UNIQUE", token) == 0) {
		*token_type = UNIQUE;	
	} else if (strcmp("UPDATE", token) == 0) {
		*token_type = UPDATE;
	} else if (strcmp("USER", token) == 0) {
		*token_type = USER;	
	} else if (strcmp("VALUES", token) == 0) {
		*token_type = VALUES;	
	} else if (strcmp("VIEW", token) == 0) {
		*token_type = VIEW;	
	} else if (strcmp("WITH", token) == 0) {
		*token_type = WITH;		
	} else if (strcmp("NULL", token) == 0) {
		*token_type = NULL0;	
	}								
	
	else {

		
		// int *integer_test[strlen(token)];
		// memset(integer_test, 1, sizeof(int));
		// // if the token is integer
		// if (token[0] == 48 && strlen(token) == 1) {
		// 	*token_type = INTEGER_NUMBER;
		// } else if (token[0] == 43 || token[0] == 45 || (token[0] <= 57 && token[0] >= 49)) {
		// 	int i;
		// 	int float_test = 0;
		// 	for (int i = 1; i < strlen(token); i++) {
		// 		if (token[i] == 46) {
		// 			integer_test[i] = 0;
		// 			int j;
		// 			// test if is a float number only including 1 . character
		// 			for (j = i; j < strlen(token); j++) {
		// 				if (token[j] < 48 || token[j] > 57) {
		// 					float_test = 1;
		// 					break;
		// 				}
		// 			}
		// 			if (float_test == 0) {
		// 				*token_type = FLOAT_NUMBER;
		// 			}
		// 			goto exit;
		// 		} else if (token[i] < 48 || token[i] > 57) {
		// 			integer_test[i] = 0;
		// 		}
		// 	}
		// 	int test = 1;
		// 	for (int m = 0; m < strlen(token); m++) {
		// 		if (integer_test[m] == 0) {
		// 			test = 0;
		// 			break;
		// 		}
		// 	}
		// 	if (test == 1) {
		// 		*token_type = INTEGER_NUMBER;
		// 	} 
		// } else if (token[0] == 46) {
		// 	int float_test = 0;
		// 	int i;
		// 	for (i = 1; i < strlen(token); i++) {
		// 		if (token[i] < 48 || token[i] > 57) {
		// 			float_test = 1;
		// 			break;
		// 		}
		// 	}
		// 	if (float_test == 0) {
		// 		*token_type = FLOAT_NUMBER;
		// 	}
		// }
		// if the token is a string starting of ' and ending of '
		if (token[0] == 39 && token[strlen(token) - 1] == 39) {
			*token_type = STRING;
		} else if (token[0] == 48 && strlen(token) == 1) {
			*token_type = INTEGER_NUMBER;
	    } else if (token[0] == 46){
		// Float number starting of .
			int i;
			int test = 0;
			for (int i = 1; i < strlen(token); i++) {
				if (token[i] < 48 || token[i] > 57) {
					test = 1;
					break;
				}
			}
			if (test == 0) {
				*token_type = FLOAT_NUMBER;
			}
		} else if (token[0] >= 48 && token[0] <= 57) {
			int i, floatmark = 0, integer_test = 0, float_test = 0;
			for (i = 1; i < strlen(token); i++) {
				if (token[i] == 46) {
					floatmark = 1;
					break;
				}
			}
			
			if (floatmark == 0) {
				for (i = 1; i < strlen(token); i++) {
					if (token[i] < 48 && token[i] > 57) {
						integer_test = 1;
						break;
					}
				}
				if (integer_test == 0) {
					*token_type = INTEGER_NUMBER;
				}
			} else {
				int i;
				for (i = 1; i < strlen(token); i++) {
					if (token[i] == 46 && i < strlen(token) - 1) {
						int j;
						for (j = i+1; j < strlen(token); j++) {
							if (token[j] < 48 || token[j] > 57) {
								float_test = 1;
								goto exit1;
							}
						}
					}
				}
				exit1:
				if (float_test == 0) {
					*token_type = FLOAT_NUMBER;
				}
			}
		} else if (token[0] == 43 || token[0] == 45) {
			if (token[1] == 46) {
				int i;
				int test = 0;
				for (int i = 1; i < strlen(token); i++) {
					if (token[j] < 48 || token[j] > 57) {
						test = 1;
						break;
					}
				}
				if (test == 0) {
					*token_type = FLOAT_NUMBER;
				}
			} else {
				int i, floatmark = 0, integer_test = 0, float_test = 0;
				for (i = 1; i < strlen(token); i++) {
					if (token[i] == 46) {
						floatmark = 1;
						break;
					}
				}
				
				if (floatmark == 0) {
					for (i = 1; i < strlen(token); i++) {
						if (token[i] < 48 && token[i] > 57) {
							integer_test = 1;
							break;
						}
					}
					if (integer_test == 0) {
						*token_type = INTEGER_NUMBER;
					}
				} else {
					int i;
					for (i = 1; i < strlen(token); i++) {
						if (token[i] == 46 && i < strlen(token) - 1) {
							int j;
							for (j = i+1; j < strlen(token); j++) {
								if (token[j] < 48 || token[j] > 57) {
									float_test = 1;
									goto exit2;
								}
							}
						}
					}
					exit2:
					if (float_test == 0) {
						*token_type = FLOAT_NUMBER;
					}
				}
			}
		// identifieR
		} else if ((token[0] >= 65 && token[0] <= 90) || (token[0] >= 97 && token[0] <= 122)) {
			int identifier_test = 0;
			int i;
			for (i = 1; i < strlen(token); i++) {
				if ((token[i] >= 48 && token[i] <= 57) || (token[i] >= 97 && token[i] <= 122) || (token[i] >= 65 && token[i] <= 90) || (token[i] == 95)) {
					identifier_test = 0;
				} else {
					identifier_test = 1;
					break;
				}
			}
			if (identifier_test == 0) {
				*token_type = IDENTIFIER;
			}
		}
	}
	return current_pos + i;
}