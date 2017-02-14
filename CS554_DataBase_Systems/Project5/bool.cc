struct MyNODE
{
	int type;	/* Can be one of FLOAT, PLUS, MINUS, MULT, DIV */
	union
	{ 
		float f;
		int i;
		char s[100];
		int *int_attr;
		double *float_attr;
		char *char_attr;
		struct MyNODE *p[2];
		struct MyNODE *q;
    }value;
    
   	int itype;	//interimidate type, can be FLOAT, INT, STRING
   	union
   	{
   		float f;
   		int i;
   		char s[100];
   		bool b;
   	}ivalue;
   	
   	int attr_size;
 };
 
struct MyNODE *a_expr;		/* the arithmetic expression */

char *last_pos;

struct MyNODE *BE();
struct MyNODE *BF();
struct MyNODE *BT();
struct MyNODE *E();
struct MyNODE *T();
struct MyNODE *F();
int find_attr = 0;


struct MyNODE *BE()
{
	struct MyNODE *help1, *help2;
	help1 = BT();
	if (help1 == NULL)
		return NULL;
	else if (help1 == ENULL)
		return ENULL;	
	while ( token_type == OR)
	{
		current = lex_anal(current, token, &token_type); // Read over '='
		help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		help2->type = OR;
		help2->value.p[0] = help1;
		help2->value.p[1] = BT();	/* Parse the right operand of mult */
		if (help2->value.p[1] == NULL)
			return(NULL);		// Error
		else if (help1 == ENULL)
			return ENULL;				
		help1 = help2;
	}
	if ( token_type == '=' || token_type == '>'
			|| token_type == '<' || token_type == LESSOREQ
			|| token_type == GREATEROREQ || token_type == NOTEQ)
	{
		printf("Syntax error: Consective comparations is not a valid boolean query.\n");
		return NULL;	
	}
			
//	current = last_pos;
	return help1;	
}

/* --------
   Parsing an boolean term: BFactor | BFactor AND BFactor
   -------- */
struct MyNODE *BT()
{
	struct MyNODE *help1, *help2;
	help1 = BF();
	if (help1 == NULL)
		return NULL;
	else if (help1 == ENULL)
		return ENULL;		
	while ( token_type == AND)
	{
		current = lex_anal(current, token, &token_type); // Read over '='
		help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		help2->type = AND;
		help2->value.p[0] = help1;
		help2->value.p[1] = BF();	/* Parse the right operand of mult */
		if (help2->value.p[1] == NULL)
			return(NULL);		// Error
		else if (help1 == ENULL)
			return ENULL;	
		help1 = help2;
	}
	return help1;	
}

/* --------
   Parsing an boolean factor: E relation operator E | NOT BTerm | (BE)
   -------- */
struct MyNODE *BF()
{
	struct MyNODE *help1, *help2;
	struct MyNODE *p;
	int type_got;
	char *bracket_pos;	
	
	if ( token_type == NOT  )
	{
        current = lex_anal(current, token, &token_type);
		p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		p->type = NOT;
		p->value.q = BF();		/* Parse the right operand of BF */
		if (p->value.q == NULL)
			return(NULL);		// Error
			
		return p;
	}         
	else if ( token_type == '('  )
	{
		bracket_pos = current;
		current = lex_anal(current, token, &token_type);
		p = BE();
		
		if (p != NULL && p != ENULL)	// parsed arithmatic operator, restore position and parsing as (E relop E)
		{
			if (token_type == ')')
			{
				current = lex_anal(current, token, &token_type);
				printf("%s\n", token);
				return(p);
			}
			else
			{
				printf("Syntax error: %s, ')' was expected.\n", token);
				return NULL;
			}
		}
		else if (p == NULL)
		{
			return NULL;
		}
		else if (p == ENULL)	//
		{
			strcpy (token, "(");
			token_type = '(';
			current = bracket_pos;
			help1 = E();

	        if (help1 == NULL)
				return(NULL);

			// token ')' after E indecation it belongs to (E) and thus make NULL return passing to (BE)		
			if (token_type == ')')
			{	
				return ENULL;		//the '( )" belong to (E)			
			}
				
			while ( token_type == '=' || token_type == '>'
					|| token_type == '<' || token_type == LESSOREQ
					|| token_type == GREATEROREQ || token_type == NOTEQ)
			{
				type_got = token_type;
				current = lex_anal(current, token, &token_type); // Read over 
				help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
				help2->type = type_got;
				help2->value.p[0] = help1;
				help2->value.p[1] = E();	/* Parse the right operand of mult */

				if (help2->value.p[1] == NULL)
					return NULL;		// Error
				help1 = help2;
			}
			return help1;	
			
		}

	}
	else
	{
        help1 = E();
        if (help1 == NULL)
			return(NULL);
		
		// token ')' after E indecation it belongs to (E) and thus make NULL return passing to (BE)		
		if (token_type == ')')
		{	
			return ENULL;		//the '( )" belong to (E)			
		}		
			
		if ( token_type == '=' || token_type == '>'
				|| token_type == '<' || token_type == LESSOREQ
				|| token_type == GREATEROREQ || token_type == NOTEQ)
		{
			type_got = token_type;
			current = lex_anal(current, token, &token_type); // Read over 
			help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
			help2->type = type_got;
			help2->value.p[0] = help1;
			help2->value.p[1] = E();	/* Parse the right operand of mult */
			if (help2->value.p[1] == NULL)
				return(NULL);		// Error
			help1 = help2;
		}
		return help1;	
	}
}

/* --------
   Parsing an arithmetic expression: Term | Term + Term | Term - Term
   -------- */
struct MyNODE *E()
{
	struct MyNODE *help1, *help2;


	help1 = T();		/* Parse a term */
	if (help1 == NULL)
	return(NULL);

//	printf("%s {342}", token);
//	printf("\n token_type = %d ", token_type);

	while ( token_type == '+' || token_type == '-' )
	{
		if (token_type == '+')
		{
			current = lex_anal(current, token, &token_type);
//printf ("token_type = %d, token = %s --- 369\n", token_type, token);
			
			help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
			help2->type = PLUS;
			help2->value.p[0] = help1;
			help2->value.p[1] = T();    /* Parse the right operand of add */
			if (help2->value.p[1] == NULL)
				return(NULL);		// Error
			help1 = help2;
			
		}
		else if (token_type == '-')
		{
			current = lex_anal(current, token, &token_type);

			help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
			help2->type = MINUS;
			help2->value.p[0] = help1;
			help2->value.p[1] = T();     /* Parse the right operand of subtract */
			if (help2->value.p[1] == NULL)
				return(NULL);		// Error
			help1 = help2;
		}
	}
    return(help1);
}


/* --------
   Parsing an arithmetic term: Factor | Factor * Factor | Factor / Factor
   -------- */
struct MyNODE *T()
{
	struct MyNODE *help1, *help2;

	help1 = F();		/* Parse a factor */
	if (help1 == NULL)
		return(NULL);
	
	while ( token_type == '*' || token_type == '/' )
	{
		if (token_type == '*')
		{ 
			current = lex_anal(current, token, &token_type); // Read over '*'
//	printf ("token_type = %d, token = %s --- 417\n", token_type, token);

			help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
			help2->type = MULT;
			help2->value.p[0] = help1;
			help2->value.p[1] = T();	/* Parse the right operand of mult */
			if (help2->value.p[1] == NULL)
				return(NULL);		// Error
			help1 = help2;
			
		}
		else if (token_type == '/')
		{ 
			current = lex_anal(current, token, &token_type);	// Read over '/'
   
			help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
			help2->type = DIV;
			help2->value.p[0] = help1;
			help2->value.p[1] = T();	/* Parse the right operand of div */
			if (help2->value.p[1] == NULL)
				return(NULL);		// Error
			help1 = help2;

		}
	}
	return(help1);
}


/* --------
   Parsing an arithmetic factor: FLOAT or (expression)
   -------- */
   
struct MyNODE *F()
{
	int i;
	struct MyNODE *p;
	
//	printf("%s {453}\n", token);

	if (token_type == INTEGER_NUMBER||token_type == FLOAT_NUMBER)
	{ 
		p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		p->type = FLOAT;
		p->value.f = atof(token);
		last_pos = current;
		current = GetOperator(current, token, &token_type);
		if (current == NULL)
		{
			printf("Syntax error: %s, A valid operator was expected.\n", token);
			return NULL;
		}
		return p;
	}
/*	else if (token_type == INT)
	{
		p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		p->type = INT;
		p->value.i = atoi(token);
		return p;
	}
*/	else if(token_type == STRING)
	{
		p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		p->type = CHAR;
		strcpy (p->value.s, token);
		last_pos = current;
		current = GetOperator(current, token, &token_type);
		if (current == NULL)
		{
			printf("Syntax error: %s, A valid operator was expected.\n", token);
			return NULL;
		}
		return p;
	}
	else if(token_type == IDENTIFIER)
	{
		if (strcmp(token, table_name) == 0)	// get relation name, expect rel.attr
		{
			current = lex_anal(current, token, &token_type);	// Read over 'TableName'
			if (token_type != '.')
			{
				cout << "Syntax error:" << token << ", '.' expected." << endl;
				return NULL;
			}
			current = lex_anal(current, token, &token_type);	// Read over '.'
		}
	
		p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
		p->type = rel->getAttrType(token);					// get attribution type
		if (p->type == -1 )
		{
			cout << "Attribution '" << token << "' not found !" << endl;
			return NULL;
		}
		else 
		{
			find_attr = 1;
			if (p->type == 'C')
			{
				p->type = CHAR_ATTR;
				p->value.char_attr = (char *)rel->getAttrPtr(token);
				p->attr_size = rel->getAttrSize(token);
			}
			else if (p->type == 'F')
			{
				p->type = FLOAT_ATTR;
				p->value.float_attr = (double *)rel->getAttrPtr(token);
			}
			else if (p->type == 'I')
			{
				p->type = INT_ATTR;
				p->value.int_attr = (int *)rel->getAttrPtr(token);
			}
		}
		last_pos = current;
		current = GetOperator(current, token, &token_type);
		if (current == NULL)
		{
			printf("Syntax error: %s, A valid operator was expected.\n", token);
			return NULL;
		}
		return p;
	}
	else if (token_type == '(')
	{ 
		current = lex_anal(current, token, &token_type);
		p = E();
		if (token_type == ')')
		{
			current = lex_anal(current, token, &token_type);
			return(p);
		}
		else
		{ 
			printf("Syntax error: %s, ')' was expected.\n", token);
			return NULL;
		}
   }
 /*  else if (token_type > 131)
   {  
		printf("Syntax error: %s, is a KEYWORD.\n", token);
		return NULL;
   }
  */ else
   {  
		printf("Syntax error: %s, A number, string, attr.name or '(' was expected.\n", token);
		return NULL;
   }
}

int Eval(struct MyNODE *e)
{
	float help;
		
	if (e->type == FLOAT)
	{
		e->ivalue.f = e->value.f;
		e->itype = FLOAT;
	}
	else if (e->type == INT)
	{
		e->ivalue.i = e->value.i;
		e->itype = INT;
	}
	else if (e->type == CHAR)
	{
		strcpy (e->ivalue.s, e->value.s);
		e->itype = CHAR;
	}
	else if (e->type == FLOAT_ATTR)
	{
		e->ivalue.f = (float)(*e->value.float_attr);
		e->itype = FLOAT;
	}
	else if (e->type == INT_ATTR)	//overload float and int
	{
		e->ivalue.f = (float)(*e->value.int_attr);
		e->itype = FLOAT;
	}
	else if (e->type == CHAR_ATTR)
	{
		memcpy (&e->ivalue.s, e->value.char_attr, e->attr_size);
		e->itype = CHAR;
	}
	else if (e->type == NOT)
	{
		Eval(e->value.q);
		if (e->value.q->itype != BOOL)
		{
			cout << "Right side of 'NOT' is not a valid boolean query, try again'" << endl;
			return 1;
		}
		else
		{
			e->itype = BOOL;
			e->ivalue.b = !e->value.q->ivalue.b;
		}
	}		
	else
	{
		Eval(e->value.p[0]);
		Eval(e->value.p[1]);
		
		if (e->type == PLUS)
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT)
					e->ivalue.f = e->value.p[0]->ivalue.f + e->value.p[1]->ivalue.f;
				else if (e->value.p[1]->itype == INT)
					e->ivalue.f = e->value.p[0]->ivalue.f + e->value.p[1]->ivalue.i;
			
					e->itype = FLOAT;
			}
			else if (e->value.p[0]->itype == INT)
			{
				if (e->value.p[1]->itype == FLOAT)
				{
					e->ivalue.f = e->value.p[0]->ivalue.i + e->value.p[1]->ivalue.f;
					e->itype = FLOAT;
				}
				else if (e->value.p[1]->itype == INT)
				{
					e->ivalue.i = e->value.p[0]->ivalue.i + e->value.p[1]->ivalue.i;
					e->itype = INT;
				}
			}
			else
			{
				cout << "'+' operator does not apply to non-numbers" << endl;
				cout <<"Invalid query, try again." << endl;
				return 1;
			}
		}
		else if (e->type == MINUS)
		{
			
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT)
					e->ivalue.f = e->value.p[0]->ivalue.f - e->value.p[1]->ivalue.f;
				else if (e->value.p[1]->itype == INT)
					e->ivalue.f = e->value.p[0]->ivalue.f - e->value.p[1]->ivalue.i;
			
					e->itype = FLOAT;
			}
			else if (e->value.p[0]->itype == INT)
			{
				if (e->value.p[1]->itype == FLOAT)
				{
					e->ivalue.f = e->value.p[0]->ivalue.i - e->value.p[1]->ivalue.f;
					e->itype = FLOAT;
				}
				else if (e->value.p[1]->itype == INT)
				{
					e->ivalue.i = e->value.p[0]->ivalue.i - e->value.p[1]->ivalue.i;
					e->itype = INT;
				}
			}
			else
			{
				cout << "'-' operator does not apply to non-numbers" << endl;
				cout <<"Invalid query, try again." << endl;
				return 1;
			}
		}
		else if (e->type == MULT)
		{
			
			if (e->value.p[1]->itype == FLOAT)
			{
				if (e->value.p[0]->itype == FLOAT)
					e->ivalue.f = e->value.p[0]->ivalue.f * e->value.p[1]->ivalue.f;
				else if (e->value.p[0]->itype == INT)
					e->ivalue.f = e->value.p[0]->ivalue.i * e->value.p[1]->ivalue.f;
			
					e->itype = FLOAT;
			}
			else if (e->value.p[1]->itype == INT)
			{
				if (e->value.p[0]->itype == FLOAT)
				{
					e->ivalue.f = e->value.p[0]->ivalue.f * e->value.p[1]->ivalue.i;
					e->itype = FLOAT;
				}
				else if (e->value.p[0]->itype == INT)
				{
					e->ivalue.i = e->value.p[0]->ivalue.i * e->value.p[1]->ivalue.i;
					e->itype = INT;
				}
			}
			else
			{
				cout << "'*' operator does not apply to non-numbers" << endl;
				cout <<"Invalid query, try again." << endl;
				return 1;
			}

		}
		else if (e->type == DIV)
		{
			
			if (e->value.p[1]->itype == FLOAT)
			{
				if (e->value.p[1]->ivalue.f == 0)
				{
					cout << "Error, divide by 0!" << endl;
					cout <<"Invalid query, try again." << endl;
				}
				else if (e->value.p[0]->itype == FLOAT)
					e->ivalue.f = e->value.p[0]->ivalue.f / e->value.p[1]->ivalue.f;
				else if (e->value.p[0]->itype == INT)
					e->ivalue.f = e->value.p[0]->ivalue.i / e->value.p[1]->ivalue.f;
			
					e->itype = FLOAT;
			}
			else if (e->value.p[1]->itype == INT)
			{
				if (e->value.p[1]->ivalue.i == 0)
				{
					cout << "Error, divide by 0!" << endl;
					cout <<"Invalid query, try again." << endl;
					return 1;
				}
				else if (e->value.p[0]->itype == FLOAT)
				{
					e->ivalue.f = e->value.p[0]->ivalue.f / e->value.p[1]->ivalue.i;
					e->itype = FLOAT;
				}
				else if (e->value.p[0]->itype == INT)
				{
					e->ivalue.i = e->value.p[0]->ivalue.i / e->value.p[1]->ivalue.i;
					e->itype = INT;
				}
			}
			else
			{
				cout << "'/' operator does not apply to non-numbers" << endl;
				cout <<"Invalid query, try again." << endl;
				return 1;
			}
		}	
		else if (e->type == '=')
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f == e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout <<"Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				if (e->value.p[1]->itype == CHAR )
				{
					e->itype = BOOL;
					e->ivalue.b = (strcmp (e->value.p[0]->ivalue.s, e->value.p[1]->ivalue.s) == 0);
				}
				else
				{
					cout << "String can not compare with numbers" << endl;
					cout <<"Invalid query, try again." << endl;
					return 1;
				}
			}
		}
		else if (e->type == '>')
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f > e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout <<"Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				cout << "Strings can only perform equal, '=', and not equal, '!=', comparison!" << endl;
				cout << "Invalid query, try again." << endl;
				return 1;
			}
		}
		else if (e->type == '<')
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f < e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout << "Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				cout << "Strings can only perform equal, '=', and not equal, '!=', comparison!" << endl;
				cout << "Invalid query, try again." << endl;
				return 1;
			}
		}
		else if (e->type == LESSOREQ)
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f <= e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout << "Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				cout << "Strings can only perform equal, '=', and not equal, '!=', comparison!" << endl;
				cout << "Invalid query, try again." << endl;
				return 1;
			}
		}
		else if (e->type == GREATEROREQ)
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f >= e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout << "Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				cout << "Strings can only perform equal, '=', and not equal, '!=', comparison!" << endl;
				cout << "Invalid query, try again." << endl;
				return 1;
			}
		}				
		else if (e->type == NOTEQ)
		{
			if (e->value.p[0]->itype == FLOAT)
			{
				if (e->value.p[1]->itype == FLOAT )	//|| == INT
				{
					e->itype = BOOL;
					e->ivalue.b = (e->value.p[0]->ivalue.f != e->value.p[1]->ivalue.f);
				}
				else
				{
					cout << "Number can not compare with strings" << endl;
					cout << "Invalid query, try again." << endl;
					return 1;
				}
			}
			else if (e->value.p[0]->itype == CHAR)
			{
				e->itype = BOOL;
				e->ivalue.b = (strcmp (e->value.p[0]->ivalue.s, e->value.p[1]->ivalue.s) != 0);
			}
		}				
		else if (e->type == AND)
		{
			if (e->value.p[0]->itype != BOOL)
			{
				cout << "Left side of 'AND' is not a valid boolean query, try again'" << endl;
				return 1;
			}
			else if (e->value.p[1]->itype != BOOL)
			{
				cout << "Right side of 'AND' is not a valid boolean query, try again'" << endl;
				return 1;
			}
			else
			{
				e->itype = BOOL;
				e->ivalue.b = (e->value.p[0]->ivalue.b && e->value.p[1]->ivalue.b);
			}
		}
		else if (e->type == OR)
		{
			if (e->value.p[0]->itype != BOOL)
			{
				cout << "Left side of 'OR' is not a valid boolean query, try again'" << endl;
				return 1;
			}
			else if (e->value.p[1]->itype != BOOL)
			{
				cout << "Right side of 'OR' is not a valid boolean query, try again'" << endl;
				return 1;
			}
			else
			{
				e->itype = BOOL;
				e->ivalue.b = (e->value.p[0]->ivalue.b || e->value.p[1]->ivalue.b);
			}
		}
		else
		{ 
			cout << "Invalid query, try again." << endl;
			return 1;
		}
	}
	return 0;
}


