#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexanal.h"

int read_command(char *input);
char *lex_anal(char *current_pos, char *token, int *token_type);