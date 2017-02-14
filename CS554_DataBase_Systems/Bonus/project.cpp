// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO
#include <fcntl.h>
#include <iostream>
#include <fstream> 
#include <cstdio> 
#include <string.h>
#include <string>
#include "lexanal.h"
#include "project1.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ScanTable.h"

using namespace std;

/********************************** Global Variables **********************************************/
int Command;            /* Store what command was parsed */     
int err; // for error indication
int record_count = 1;
string this_table_name; // table name
string drop_table_name; // drop table name
char data_types[100]; // data_types array
int data_length[100]; 
std::string attribute_names[100]; // attribute names string array
char attribute_data[1000];      /* For input data */
char *last_pos;

char *current_location;
char* current_token;
int current_type, fd;
int select_star_case = 0;

int sss = 0;
// define the command type enum
enum COMMAND_TYPE {
    COMMAND_CREATE = 818,
    COMMAND_DROP, COMMAND_INSERT, COMMAND_DELETE, COMMAND_SELECT, COMMAND_ILLEGAL
} ;

enum ERROR_TYPE {
    NO_ERROR = 919,
    PARSE_ERROR, PROCESS_ERROR 
} ;

enum TYPE { PLUS, MINUS, MULT, DIV, CHAR_ATTR, FLOAT_ATTR, INT_ATTR, BOOL };

struct MyNODE { 
    int type;               /* Contain type of node */
    
    union { 
        int   i;                  /* INT_CONST */
        double f;                  /* FLOAT_CONST */
        char  s[100];             /* STRING_CONST */

        int   * int_attr;           /* Attribute of type INT */
        double * float_attr;           /* Attribute of type FLOAT */
        char  * char_attr;           /* Attribute of type CHAR */


        struct MyNODE * p[2];       /* Points to operands of a
                      binary arithmetic, relational 
        or logic operation. */
        struct MyNODE * q;          /* Points to operand of a
                      uniary arithmetic (e.g.: -x)
          or logic (e.g.: NOT x) operation */
    } value;   
          
        int type_tmp;  //interimidate type, can be FLOAT, INT, STRING, BOOLEAN
        union {
          double f;
          int i;
          char s[100];
          bool b;
        } ivalue;
    
        int attr_size;
};

struct MyNODE *BE(struct select_structure *selo);
struct MyNODE *BF(struct select_structure *selo);
struct MyNODE *BT(struct select_structure *selo);
struct MyNODE *E(struct select_structure *selo);
struct MyNODE *T(struct select_structure *selo);
struct MyNODE *F(struct select_structure *selo);
int parseSelect(struct select_structure *selo);
struct MyNODE *ENULL = (MyNODE *)(-1);
ScanTable *rel;



// data structure for SELECT
struct select_structure {
    int  NAttrs;             // Number of attrs                    
    char AttrName[100][32];  // Attribute name
    MyNODE *Attr[100];         // Buffer in ScanTable to get the attr value

    int  NRels;              // Number of relations
    int  rel_aliased[10];    
    char RelName[20][32];    // Relation name (optional, you can use
                           // the TableName[ ] field in the ScanTable
               // to store the Relation Name...
    ScanTable *Rel[10];      // ScanTable object for the relation

    MyNODE *where;             // Where clause

    /* =========================================
     The following 2 variables are used by 
    the optional part of this project
    ========================================= */    
    select_structure *parent;       // Parent and Child are used to
    select_structure *child;        // implement nested queries
    
    int in_type;
    int in_count;
    int num_res[30];
    char string_res[30][32];
};


void ProcessSelect(struct MyNODE *where, int N_Relations, int currRel, struct select_structure *selo);
char* inp_saved; 
int n_saved;

char *saved_token;
char *saved_current;
int saved_type;

char* current_inp;
int current_n;
/*************************************************************************************************/

/****************************** Delete Helper Function *******************************************/

struct MyNODE *BE(struct select_structure *selo) {
    struct MyNODE *help1, *help2;
    help1 = BT(selo);
    if (help1 == NULL) {
        return NULL;
    }
    else if (help1 == ENULL) {
        return ENULL;
    }
    while (current_type == OR) {
        current_location = lex_anal(current_location, current_token, &current_type); 
        current_location = lex_anal(current_location, current_token, &current_type); 
        help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
        help2->type = OR;
        help2->value.p[0] = help1;
        help2->value.p[1] = BT(selo);   
        if (help2->value.p[1] == NULL)
            return(NULL);      
        else if (help1 == ENULL)
            return ENULL;               
        help1 = help2;
    }
    if ( current_type == '=' || current_type == '>'
            || current_type == '<' || current_type == LESSOREQ
            || current_type == GREATEROREQ || current_type == NOTEQ) {
        
        printf("Syntax error: Consective comparations is not a valid boolean query.\n");
        return NULL;    
    }
            
    return help1;   
}


struct MyNODE *BT(struct select_structure *selo) {
    struct MyNODE *help1, *help2;
    char *token_temp;
    int type_tmp;
    
    help1 = BF(selo);
    if (help1 == NULL) {
        return NULL;
    } else if (help1 == ENULL) {
        return ENULL;
    }
    
    while (current_type == AND) {
        current_location = lex_anal(current_location, current_token, &current_type); 
        current_location = lex_anal(current_location, current_token, &current_type); 
        
        help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
        help2->type = AND;
        help2->value.p[0] = help1;
        help2->value.p[1] = BF(selo);   
        if (help2->value.p[1] == NULL)
            return(NULL);       
        else if (help1 == ENULL)
            return ENULL;   
        help1 = help2;
    }
    
    return help1;   
}


struct MyNODE *BF(struct select_structure *selo) {
    struct MyNODE *help1, *help2;
    struct MyNODE *p;
    int type_p;
    char *bracket_pos;  
    
    if ( current_type == NOT ) {
        current_location = lex_anal(current_location, current_token, &current_type);
        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
        p->type = NOT;
        p->value.q = BF(selo);      /* Parse the right operand of BF */
        if (p->value.q == NULL)
            return(NULL);       // Error
            
        return p;
    } else if ( current_type == '(' ) {
        bracket_pos = current_location;
        current_location = lex_anal(current_location, current_token, &current_type);
        p = BE(selo);
        if (p != NULL && p != ENULL) {
            if (current_type == ')') {    
                current_location = lex_anal(current_location, current_token, &current_type);
                current_location = peek(current_location, current_token, &current_type);
                return(p);
            } else {
                printf("Syntax error: %s, ')' was expected.\n", current_token);
                return NULL;
            }
        }
        else if (p == NULL) {
            return NULL;
        }
        else if (p == ENULL) {
            strcpy (current_token, "(");
            current_type = '(';
            current_location = bracket_pos;
            help1 = E(selo);
            
            if (help1 == NULL) {
                return(NULL);
            }
                    
            // current_token ')' after E indecation it belongs to (E) and thus make NULL return passing to (BE)     
            if (current_type == ')') {    
                current_location = lex_anal(current_location, current_token, &current_type); // Read over        
            }
                
            while ( current_type == '=' || current_type == '>'
                    || current_type == '<' || current_type == LESSOREQ
                    || current_type == GREATEROREQ || current_type == NOTEQ) {
                type_p = current_type;
                current_location = lex_anal(current_location, current_token, &current_type); // Read over 
                help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                help2->type = type_p;
                help2->value.p[0] = help1;
                help2->value.p[1] = E(selo);    

                if (help2->value.p[1] == NULL)
                    return NULL;        // Error
                help1 = help2;
            }
            return help1;   
            
        }

    } else {
        help1 = E(selo);
        if (help1 == NULL)
            return(NULL);
         
        if (current_type == ')') {   
            return ENULL;       //the '( )" belong to (E)
    /*******************************************************************************************************************************************************************/
        } else if (current_type == IN) {
            struct select_structure *selo_next;
            selo_next = (struct select_structure *) malloc(sizeof(struct select_structure));
            selo->child = selo_next;
            selo_next->parent = selo;

            current_location = lex_anal(current_location, current_token, &current_type); // Read over 
            current_location = lex_anal(current_location, current_token, &current_type); // Read over 
            
            if (current_type != '(') {
                printf("Syntax error: %s, '(' was expected.\n", current_token);
                return NULL;
            }
            current_location = lex_anal(current_location, current_token, &current_type); // Read over            
            if (current_type != SELECT) {
                printf("Syntax error: %s, keyword SELECT was expected.\n", current_token);
                return NULL;
            }
            
            int result = parseSelect(selo_next);

            if (result == 0) {
                help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                help2->type = IN;
                help2->value.p[0] = help1;
                help1 = help2;
                return help1;
            } else {
                printf("Error: Could not process the IN command...\n");
                return NULL;                
            }
        }
    /*******************************************************************************************************************************************************************/    
        if ( current_type == '=' || current_type == '>'
                || current_type == '<' || current_type == LESSOREQ
                || current_type == GREATEROREQ || current_type == NOTEQ) {
            type_p = current_type;
            current_location = lex_anal(current_location, current_token, &current_type); // Read over 
            current_location = lex_anal(current_location, current_token, &current_type);
            
            help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            help2->type = type_p;
            help2->value.p[0] = help1;
            help2->value.p[1] = E(selo);   
            if (help2->value.p[1] == NULL)
                return(NULL);       // Error
            help1 = help2;
        }
        return help1;   
    }
    
    return NULL;
}

struct MyNODE *E(struct select_structure *selo) {
    struct MyNODE *help1, *help2;
    help1 = T(selo);        
    if (help1 == NULL)
        return(NULL);

    while ( current_type == '+' || current_type == '-' ) {
        if (current_type == '+') {
            current_location = lex_anal(current_location, current_token, &current_type);
            current_location = lex_anal(current_location, current_token, &current_type);
            help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            help2->type = PLUS;
            help2->value.p[0] = help1;
            help2->value.p[1] = T(selo);    /* Parse the right operand of add */
            if (help2->value.p[1] == NULL)
                return(NULL);       // Error
            help1 = help2;
            
        }
        else if (current_type == '-') {
            current_location = lex_anal(current_location, current_token, &current_type);
            current_location = lex_anal(current_location, current_token, &current_type);
            help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            help2->type = MINUS;
            help2->value.p[0] = help1;
            help2->value.p[1] = T(selo);     
            if (help2->value.p[1] == NULL)
                return(NULL);       // Error
            help1 = help2;
        }
    }
    return(help1);
}


struct MyNODE *T(struct select_structure *selo) {
    struct MyNODE *help1, *help2;

    help1 = F(selo);        
    if (help1 == NULL)
        return(NULL);

    while ( current_type == '*' || current_type == '/' ) {
        if (current_type == '*') { 
            current_location = lex_anal(current_location, current_token, &current_type); // Read over '*'
            current_location = lex_anal(current_location, current_token, &current_type);

            help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            help2->type = MULT;
            help2->value.p[0] = help1;
            help2->value.p[1] = T(selo);    /* Parse the right operand of mult */
            if (help2->value.p[1] == NULL)
                return(NULL);       // Error
            help1 = help2;
            
        } else if (current_type == '/') { 
            current_location = lex_anal(current_location, current_token, &current_type);    // Read over '/'
            current_location = lex_anal(current_location, current_token, &current_type);
            
            help2 = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            help2->type = DIV;
            help2->value.p[0] = help1;
            help2->value.p[1] = T(selo);    
            if (help2->value.p[1] == NULL)
                return(NULL);       
            help1 = help2;

        }
    }
    return(help1);
}

   
struct MyNODE *F(struct select_structure *selo) {
    int i, index;
    struct MyNODE *p; 
    if (current_type == INTEGER_NUMBER||current_type == FLOAT_NUMBER) { 
        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
        p->type = FLOAT;
        p->value.f = atof(current_token);
        last_pos = current_location;
        current_location = peek(current_location, current_token, &current_type);
        if (current_location == NULL) {
            printf("Syntax error: %s, A valid operator was expected.\n", current_token);
            return NULL;
        }
        return p;
    }

    else if(current_type == STRING) {
        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
        p->type = CHAR;
        strcpy (p->value.s, current_token);
        last_pos = current_location;
        current_location = peek(current_location, current_token, &current_type);
        if (current_location == NULL) {
            printf("Syntax error: %s, A valid operator was expected.\n", current_token);
            return NULL;
        }
        return p;
    } else if(current_type == IDENTIFIER) { 
        // here to parse the DELETE COMMAND
        if (Command == COMMAND_DELETE) {
            if (strcmp(current_token, this_table_name.c_str()) == 0) {
                current_location = lex_anal(current_location, current_token, &current_type);    // Read over 'TableName'
                if (current_type != '.') {
                    cout << "Syntax error:" << current_token << ", '.' expected." << endl;
                    return NULL;
                }
                current_location = lex_anal(current_location, current_token, &current_type);    // Read over '.'
            }
        
            p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
            p->type = rel->getAttrType(current_token);                  // get attribution type
            if (p->type == -1 ) {
                cout << "Attribution '" << current_token << "' not found !" << endl;
                return NULL;
            }
            else {
                if (p->type == 'C') {
                    p->type = CHAR_ATTR;
                    p->value.char_attr = (char *)rel->getAttrPtr(current_token);
                    p->attr_size = rel->getAttrSize(current_token);
                }
                else if (p->type == 'F') {
                    p->type = FLOAT_ATTR;
                    p->value.float_attr = (double *)rel->getAttrPtr(current_token);
                }
                else if (p->type == 'I') {
                    p->type = INT_ATTR;
                    p->value.int_attr = (int *)rel->getAttrPtr(current_token);
                }
            }
            last_pos = current_location;
            current_location = peek(current_location, current_token, &current_type);
            if (current_location == NULL) {
                printf("Syntax error: %s, A valid operator was expected.\n", current_token);
                return NULL;
            }
        // here to parse the SELECT command
        } else if (Command == COMMAND_SELECT) {
            if (select_star_case) {
                int attr_count = 0;
                int attr_loc;
                int j = selo->NRels;
                // try to find an error
                while (j >= 0) {
                    p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                    p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                    if (p->type != -1) {
                        attr_count++;
                        break;
                    }
                    j--;                    
                }
                // if there are no such attributes
                if (j <= -1) {
                    cout << "Error: Ambiguous! Attribution '" << current_token << "' not found in any relation!" << endl;
                    return NULL;                    
                }
                
                attr_count = 0;
                j = selo->NRels;
                while (j >= 0) {
                    p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                    p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                    if (p->type != -1) {
                        attr_count++;
                    }
                    j--;                    
                }
                // if there are no such attributes
                if (attr_count > 1) {
                    cout << "Error: Ambiguous! Attribution '" << current_token << "' found in more than relation!" << endl;
                    return NULL;                    
                }
                                                
                // try to find that attribute
                attr_count = 0;
                j = selo->NRels;
                while (j >= 0) {
                    p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                    p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                    if (p->type != -1) {
                        break;
                    }
                    j--;                    
                }
                
                if (p->type == 'C') {
                    p->type = CHAR_ATTR;
                    p->value.char_attr = (char *)((selo->Rel[j])->getAttrPtr(current_token));
                    p->attr_size = (selo->Rel[j])->getAttrSize(current_token);
                }
                else if (p->type == 'F') {
                    p->type = FLOAT_ATTR;
                    p->value.float_attr = (double *)((selo->Rel[j])->getAttrPtr(current_token));
                }
                else if (p->type == 'I') {
                    p->type = INT_ATTR;
                    p->value.int_attr = (int *)((selo->Rel[j])->getAttrPtr(current_token));
                }  
                
                return p;
            } else {
                for (index = 0; index < selo->NRels + 1; index++) {
                    if (selo->rel_aliased[index]) {
                        if (strcmp(current_token, selo->RelName[index]) == 0) {
                            printf("Error: the Relation name %s has been aliased, thus it is cannot be used anymore...\n", current_token);
                            return NULL;                        
                        } else if (strcmp(current_token, selo->RelName[index + 10]) == 0) {
                            current_location = lex_anal(current_location, current_token, &current_type);    // Read over 'TableName'
                            if (current_type != '.') {
                                cout << "Syntax error:" << current_token << ", '.' expected." << endl;
                                return NULL;
                            }
                            current_location = lex_anal(current_location, current_token, &current_type);    // Read over '.'
                            break;
                        }
                    } else {
                        if (strcmp(current_token, selo->RelName[index + 10]) == 0) {
                            printf("Error: the Relation name %s has not been aliased, thus it is cannot be used...\n", current_token);
                            return NULL; 
                        } else if  (strcmp(current_token, selo->RelName[index]) == 0) {
                            current_location = lex_anal(current_location, current_token, &current_type);    // Read over 'TableName'
                            if (current_type != '.') {
                                cout << "Syntax error:" << current_token << ", '.' expected." << endl;
                                return NULL;
                            }
                            current_location = lex_anal(current_location, current_token, &current_type);    // Read over '.'
                            break;                        
                        }                  
                    }
                }

                if (index == selo->NRels + 1) {
                // there appears no relation name, then do a loop to find the attribute
                    int attr_count = 0;
                    int attr_loc;
                    int j = selo->NRels;
                    // try to find an error
                    while (j >= 0) {
                        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                        p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                        if (p->type != -1) {
                            attr_count++;
                            break;
                        }
                        j--;                    
                    }
                    
                    // if there are no such attributes
                    if (j <= -1) {
                        cout << "Error: Ambiguous! Attribution '" << current_token << "' not found in any relation!" << endl;
                        return NULL;                    
                    }
                    
                    attr_count = 0;
                    j = selo->NRels;
                    while (j >= 0) {
                        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                        p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                        if (p->type != -1) {
                            attr_count++;
                        }
                        j--;                    
                    }
                    // if there are no such attributes
                    if (attr_count > 1) {
                        cout << "Error: Ambiguous! Attribution '" << current_token << "' found in more than relation!" << endl;
                        return NULL;                    
                    }
                    // try to find that attribute
                    attr_count = 0;
                    j = selo->NRels;
                    while (j >= 0) {
                        p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                        p->type = selo->Rel[j]->getAttrType(current_token);                  // get attribution type
                        if (p->type != -1) {
                            break;
                        }
                        j--;                    
                    }

                    if (p->type == 'C') {
                        p->type = CHAR_ATTR;
                        p->value.char_attr = (char *)((selo->Rel[j])->getAttrPtr(current_token));
                        p->attr_size = (selo->Rel[j])->getAttrSize(current_token);
                    }
                    else if (p->type == 'F') {
                        p->type = FLOAT_ATTR;
                        p->value.float_attr = (double *)((selo->Rel[j])->getAttrPtr(current_token));
                    }
                   
                    else if (p->type == 'I') {
                        p->type = INT_ATTR;
                        p->value.int_attr = (int *)((selo->Rel[j])->getAttrPtr(current_token));
                    }  
                                 
                    current_location = peek(current_location, current_token, &current_type);
                    if (current_location == NULL) {
                        printf("Syntax error: %s, A valid operator was expected.\n", current_token);
                        return NULL;
                    }                            
                } else {
                    p = (struct MyNODE *) malloc(sizeof(struct MyNODE));
                    p->type = selo->Rel[index]->getAttrType(current_token);                  // get attribution type
                    if (p->type == -1 ) {
                        cout << "Attribution '" << current_token << "' not found in " << selo->RelName[index] << "!" << endl;
                        return NULL;
                    }
                    else {
                        if (p->type == 'C') {
                            p->type = CHAR_ATTR;
                            p->value.char_attr = (char *)((selo->Rel[index])->getAttrPtr(current_token));
                            p->attr_size = (selo->Rel[index])->getAttrSize(current_token);
                        }
                        else if (p->type == 'F') {
                            p->type = FLOAT_ATTR;
                            p->value.float_attr = (double *)((selo->Rel[index])->getAttrPtr(current_token));
                        }
                        else if (p->type == 'I') {
                            p->type = INT_ATTR;
                            p->value.int_attr = (int *)((selo->Rel[index])->getAttrPtr(current_token));
                        }
                    }

                    current_location = peek(current_location, current_token, &current_type);
                    if (current_location == NULL) {
                        printf("Syntax error: %s, A valid operator was expected.\n", current_token);
                        return NULL;
                    } 
                }
            }
        }
    } else if (current_type == '(') { 
        current_location = lex_anal(current_location, current_token, &current_type);
        p = E(selo);
        if (current_type == ')') {
            current_location = lex_anal(current_location, current_token, &current_type);
            return(p);
        } else { 
            printf("Syntax error: %s, ')' was expected.\n", current_token);
            return NULL;
        }
   } else {  
        printf("Syntax error: %s, A number, string, attr.name or '(' was expected.\n", current_token);
        return NULL;
   }
   
   return p;
}

int Eval(struct MyNODE *e, struct select_structure *selo)
{
    double help;
    
    if (e->type == IN) {
        if (e->value.p[0]->type == INT_ATTR) {
            if (selo->child->in_type != INT) {
                cout << "Error: IN Command, compared type is not compatible..." << endl;
                return 1;
            }
            int test = 0;
            e->type_tmp = BOOL;
            for (int i = 0; i < selo->child->in_count; i++) {
                if (*(e->value.p[0]->value.int_attr) == selo->child->num_res[i]) {
                    test = 1;
                    break;
                }
            }
            if (test) {
                e->ivalue.b = 1;
            } else {
                e->ivalue.b = 0;
            }
            
        } else if (e->value.p[0]->type == FLOAT_ATTR) {
            if (selo->child->in_type != FLOAT) {
                cout << "Error: IN Command, compared type is not compatible..." << endl;
                return 1;
            } 
            int test = 0;
            e->type_tmp = BOOL;
            for (int i = 0; i < selo->child->in_count; i++) {
                if ((int)(*(e->value.p[0]->value.float_attr)) == selo->child->num_res[i]) {
                    test = 1;
                    break;
                }
            }
            if (test) {
                e->ivalue.b = 1;
            } else {
                e->ivalue.b = 0;
            }
                   
        } else if (e->value.p[0]->type == CHAR_ATTR) {
            if (selo->child->in_type != CHAR) {
                cout << "Error: IN Command, compared type is not compatible..." << endl;
                return 1;
            }
            int test = 0;
            e->type_tmp = BOOL;
            for (int i = 0; i < selo->child->in_count; i++) {
                if (strcmp(e->value.p[0]->value.char_attr, selo->child->string_res[i]) == 0) {
                    test = 1;
                    break;
                }
            }
            if (test) {
                e->ivalue.b = 1;
            } else {
                e->ivalue.b = 0;
            }
                         
        }
    }
    else if (e->type == FLOAT)
    {
        e->ivalue.f = e->value.f;
        e->type_tmp = FLOAT;
    }
    else if (e->type == INT)
    {
        e->ivalue.i = e->value.i;
        e->type_tmp = INT;
    }
    else if (e->type == CHAR)
    {
        strcpy (e->ivalue.s, e->value.s);
        e->type_tmp = CHAR;
    }
    else if (e->type == FLOAT_ATTR)
    {
        e->ivalue.f = (double)(*e->value.float_attr);
        e->type_tmp = FLOAT;
    }
    else if (e->type == INT_ATTR)   //overload double and int
    {
        e->ivalue.f = (double)(*e->value.int_attr);
        e->type_tmp = FLOAT;
    }
    else if (e->type == CHAR_ATTR)
    {
        memcpy (&e->ivalue.s, e->value.char_attr, e->attr_size);
        e->type_tmp = CHAR;
    }
    else if (e->type == NOT)
    {
        Eval(e->value.q, selo);
        if (e->value.q->type_tmp != BOOL)
        {
            cout << "Right side of 'NOT' is not a valid boolean query, try again" << endl;
            return 1;
        }
        else
        {
            e->type_tmp = BOOL;
            e->ivalue.b = !e->value.q->ivalue.b;
        }
    }       
    else
    {
        Eval(e->value.p[0], selo);
        Eval(e->value.p[1], selo);
        
        if (e->type == PLUS)
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT)
                    e->ivalue.f = e->value.p[0]->ivalue.f + e->value.p[1]->ivalue.f;
                else if (e->value.p[1]->type_tmp == INT)
                    e->ivalue.f = e->value.p[0]->ivalue.f + e->value.p[1]->ivalue.i;
            
                    e->type_tmp = FLOAT;
            }
            else if (e->value.p[0]->type_tmp == INT)
            {
                if (e->value.p[1]->type_tmp == FLOAT)
                {
                    e->ivalue.f = e->value.p[0]->ivalue.i + e->value.p[1]->ivalue.f;
                    e->type_tmp = FLOAT;
                }
                else if (e->value.p[1]->type_tmp == INT)
                {
                    e->ivalue.i = e->value.p[0]->ivalue.i + e->value.p[1]->ivalue.i;
                    e->type_tmp = INT;
                }
            }
            else
            {
                cout << "'+' operator does not apply to non-numbers" << endl;
                return 1;
            }
        }
        else if (e->type == MINUS)
        {
            
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT)
                    e->ivalue.f = e->value.p[0]->ivalue.f - e->value.p[1]->ivalue.f;
                else if (e->value.p[1]->type_tmp == INT)
                    e->ivalue.f = e->value.p[0]->ivalue.f - e->value.p[1]->ivalue.i;
            
                    e->type_tmp = FLOAT;
            }
            else if (e->value.p[0]->type_tmp == INT)
            {
                if (e->value.p[1]->type_tmp == FLOAT)
                {
                    e->ivalue.f = e->value.p[0]->ivalue.i - e->value.p[1]->ivalue.f;
                    e->type_tmp = FLOAT;
                }
                else if (e->value.p[1]->type_tmp == INT)
                {
                    e->ivalue.i = e->value.p[0]->ivalue.i - e->value.p[1]->ivalue.i;
                    e->type_tmp = INT;
                }
            }
            else
            {
                cout << "'-' operator does not apply to non-numbers" << endl;
                return 1;
            }
        }
        else if (e->type == MULT)
        {
            
            if (e->value.p[1]->type_tmp == FLOAT)
            {
                if (e->value.p[0]->type_tmp == FLOAT)
                    e->ivalue.f = e->value.p[0]->ivalue.f * e->value.p[1]->ivalue.f;
                else if (e->value.p[0]->type_tmp == INT)
                    e->ivalue.f = e->value.p[0]->ivalue.i * e->value.p[1]->ivalue.f;
            
                    e->type_tmp = FLOAT;
            }
            else if (e->value.p[1]->type_tmp == INT)
            {
                if (e->value.p[0]->type_tmp == FLOAT)
                {
                    e->ivalue.f = e->value.p[0]->ivalue.f * e->value.p[1]->ivalue.i;
                    e->type_tmp = FLOAT;
                }
                else if (e->value.p[0]->type_tmp == INT)
                {
                    e->ivalue.i = e->value.p[0]->ivalue.i * e->value.p[1]->ivalue.i;
                    e->type_tmp = INT;
                }
            }
            else
            {
                cout << "'*' operator does not apply to non-numbers" << endl;
                return 1;
            }

        }
        else if (e->type == DIV)
        {
            
            if (e->value.p[1]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->ivalue.f == 0)
                {
                    cout << "Error, divide by 0!" << endl;
                }
                else if (e->value.p[0]->type_tmp == FLOAT)
                    e->ivalue.f = e->value.p[0]->ivalue.f / e->value.p[1]->ivalue.f;
                else if (e->value.p[0]->type_tmp == INT)
                    e->ivalue.f = e->value.p[0]->ivalue.i / e->value.p[1]->ivalue.f;
            
                    e->type_tmp = FLOAT;
            }
            else if (e->value.p[1]->type_tmp == INT)
            {
                if (e->value.p[1]->ivalue.i == 0)
                {
                    cout << "Error, divide by 0!" << endl;
                    return 1;
                }
                else if (e->value.p[0]->type_tmp == FLOAT)
                {
                    e->ivalue.f = e->value.p[0]->ivalue.f / e->value.p[1]->ivalue.i;
                    e->type_tmp = FLOAT;
                }
                else if (e->value.p[0]->type_tmp == INT)
                {
                    e->ivalue.i = e->value.p[0]->ivalue.i / e->value.p[1]->ivalue.i;
                    e->type_tmp = INT;
                }
            }
            else
            {
                cout << "'/' operator does not apply to non-numbers" << endl;
                return 1;
            }
        }   
        else if (e->type == '=')
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f == e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                if (e->value.p[1]->type_tmp == CHAR )
                {

                    string x = string(e->value.p[0]->ivalue.s);
                    string y = string(e->value.p[1]->ivalue.s);
                    if (x.at(0) != '\'') {
                        string temp = "\'" + x + "\'";
                        x = temp;
                    }
                    if (y.at(0) != '\'') {
                        string temp = "\'" + y + "\'";
                        y = temp;
                    }
                     
                    e->type_tmp = BOOL;
                    e->ivalue.b = !(x.compare(y));
                }
                else
                {
                    cout << "String can not compare with numbers" << endl;
                    return 1;
                }
            }
        }
        else if (e->type == '>')
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f > e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                if (e->value.p[1]->type_tmp == FLOAT || e->value.p[1]->type_tmp == INT ) //|| == INT
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
                else {
                    e->type_tmp = BOOL;
                    string x = string(e->value.p[0]->ivalue.s);
                    string y = string(e->value.p[1]->ivalue.s);
                    if (x.at(0) != '\'') {
                        string temp = "\'" + x + "\'";
                        x = temp;
                    }
                    if (y.at(0) != '\'') {
                        string temp = "\'" + y + "\'";
                        y = temp;
                    }                   
                    
                    e->ivalue.b = (x > y);
                }
            }
        }
        else if (e->type == '<')
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f < e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                if (e->value.p[1]->type_tmp == FLOAT || e->value.p[1]->type_tmp == INT ) //|| == INT
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
                else {
                    e->type_tmp = BOOL;
                    string x = string(e->value.p[0]->ivalue.s);
                    string y = string(e->value.p[1]->ivalue.s);
                    if (x.at(0) != '\'') {
                        string temp = "\'" + x + "\'";
                        x = temp;
                    }
                    if (y.at(0) != '\'') {
                        string temp = "\'" + y + "\'";
                        y = temp;
                    }                   
                    
                    e->ivalue.b = (x < y);
                }
            }
        }
        else if (e->type == LESSOREQ)
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f <= e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                if (e->value.p[1]->type_tmp == FLOAT || e->value.p[1]->type_tmp == INT ) //|| == INT
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
                else {
                    e->type_tmp = BOOL;
                    string x = string(e->value.p[0]->ivalue.s);
                    string y = string(e->value.p[1]->ivalue.s);
                    if (x.at(0) != '\'') {
                        string temp = "\'" + x + "\'";
                        x = temp;
                    }
                    if (y.at(0) != '\'') {
                        string temp = "\'" + y + "\'";
                        y = temp;
                    }                   
                    
                    e->ivalue.b = (x <= y);
                }
            }
        }
        else if (e->type == GREATEROREQ)
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f >= e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                if (e->value.p[1]->type_tmp == FLOAT || e->value.p[1]->type_tmp == INT ) //|| == INT
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
                else {
                    e->type_tmp = BOOL;
                    string x = string(e->value.p[0]->ivalue.s);
                    string y = string(e->value.p[1]->ivalue.s);
                    if (x.at(0) != '\'') {
                        string temp = "\'" + x + "\'";
                        x = temp;
                    }
                    if (y.at(0) != '\'') {
                        string temp = "\'" + y + "\'";
                        y = temp;
                    }                   
                    
                    e->ivalue.b = (x >= y);
                }
            }
        }               
        else if (e->type == NOTEQ)
        {
            if (e->value.p[0]->type_tmp == FLOAT)
            {
                if (e->value.p[1]->type_tmp == FLOAT ) //|| == INT
                {
                    e->type_tmp = BOOL;
                    e->ivalue.b = (e->value.p[0]->ivalue.f != e->value.p[1]->ivalue.f);
                }
                else
                {
                    cout << "Number can not compare with strings" << endl;
                    return 1;
                }
            }
            else if (e->value.p[0]->type_tmp == CHAR)
            {
                e->type_tmp = BOOL;
                e->ivalue.b = (strcmp (e->value.p[0]->ivalue.s, e->value.p[1]->ivalue.s) != 0);
            }
        }               
        else if (e->type == AND)
        {
            if (e->value.p[0]->type_tmp != BOOL)
            {
                cout << "Left side of 'AND' is not a valid boolean query, try again" << endl;
                return 1;
            }
            else if (e->value.p[1]->type_tmp != BOOL)
            {
                cout << "Right side of 'AND' is not a valid boolean query, try again" << endl;
                return 1;
            }
            else
            {
                e->type_tmp = BOOL;
                e->ivalue.b = (e->value.p[0]->ivalue.b && e->value.p[1]->ivalue.b);
            }
        }
        else if (e->type == OR)
        {
            if (e->value.p[0]->type_tmp != BOOL)
            {
                cout << "Left side of 'OR' is not a valid boolean query, try again" << endl;
                return 1;
            }
            else if (e->value.p[1]->type_tmp != BOOL)
            {
                cout << "Right side of 'OR' is not a valid boolean query, try again" << endl;
                return 1;
            }
            else
            {
                e->type_tmp = BOOL;
                e->ivalue.b = (e->value.p[0]->ivalue.b || e->value.p[1]->ivalue.b);
            }
        }
        else
        { 
            cout << "Error : Invalid query, try again." << endl;
            return 1;
        }
    }
    return 0;
}



/********************************** Parser Function **********************************************/

// function to parse the following commands following CREATE
int parseCreate(char *current, char* inp, int n, char* token, int token_type) {
  if (current == inp + n) {
        printf("Error: keyword TABLE expected\n");
        return PARSE_ERROR;
  }
  while ( current != inp + n ) { // has not reached the end of command
    while ( (current != inp + n) && isspace(*current)) {
        current++;      // Skip space
    }
    
    current = lex_anal(current, token, &token_type);   // Get the "table" command

    if (strcmp(token, "TABLE") != 0) {
        printf("Error: keyword TABLE expected\n");
        return PARSE_ERROR;
    }
    
    while ( (current != inp + n) && isspace(*current) )
        current++;            // Skip space
    current = lex_anal(current, token, &token_type);   // Get table name
    if (token_type != IDENTIFIER) {
        printf("Error: table name expected\n");
        return PARSE_ERROR;
    }
    this_table_name = token;

    while ( (current != inp + n) && isspace(*current) )
        current++;            // Skip space
    current = lex_anal(current, token, &token_type);   // Get the '(' symbol
    if (strcmp("(", token)) {
        printf("Error : '(' expected\n");
        return PARSE_ERROR;
    } 
    
    // start to parse the contents of table
    do {
        // get the attribute name
        while ( (current != inp + n) && isspace(*current) )
            current++;            // Skip space
        current = lex_anal(current, token, &token_type);
        if (token_type != IDENTIFIER) {
            printf("Error '%s': attribute name expected\n", token);
            return PARSE_ERROR;
        }
        attribute_names[record_count] = token;
          
        // get the data type
        while ( (current != inp + n) && isspace(*current) )
            current++;            // Skip space
        current = lex_anal(current, token, &token_type);
        string token_string = string(token);
        for(int i = 0; i < strlen(token); i++) {
            token[i] = toupper(token[i]);
        }
        if (strcmp(token, "INT") == 0) {
            data_types[record_count] = 'I';
            data_length[record_count] = 4;
        } else if (strcmp(token,"FLOAT") == 0) {
            data_types[record_count] = 'F';
            data_length[record_count] = 8;
        } else if (strcmp(token, "CHAR") == 0) { 
            while ( (current != inp + n) && isspace(*current) )
                current++;            // Skip space
            current = lex_anal(current, token, &token_type);
            if (strcmp(token, "(")) {
                printf("Error : '(' expected\n");
                return PARSE_ERROR;
            }
            
            while ( (current != inp + n) && isspace(*current) )
                current++;            // Skip space
            current = lex_anal(current, token, &token_type);
            
            if (token_type == INTEGER_NUMBER) {
                data_types[record_count] = 'C';
                data_length[record_count] = atoi(token);
            } else {
                printf("Error: integer size was expected\n");
                return PARSE_ERROR;         
            }
            
            while ( (current != inp + n) && isspace(*current) )
                current++;            // Skip space
            current = lex_anal(current, token, &token_type);
            
            if (strcmp(token, ")")) {
                printf("')' expected\n");
                return PARSE_ERROR;
            }
        } else {
            printf("Error '%s': Unexpected data types\n", token);
            return PARSE_ERROR;
        }
        record_count++;
        
        while ( (current != inp + n) && isspace(*current) )
            current++;            // Skip space
        current = lex_anal(current, token, &token_type);   // must be "," or ")"
        } while (strcmp(token, ",") == 0);

        if (strcmp(")", token) != 0) {
            printf("Error : ')' expected\n");
            return PARSE_ERROR;
        }
    }
    
  return NO_ERROR;
}

int parseDrop(char *current, char* inp, int n, char* token, int token_type) {
  if (current == inp + n) {
        printf("Error: keyword Table expected\n");
        return PARSE_ERROR;
  }
  
    while ( (current != inp + n) && isspace(*current)) {
        current++;      // Skip space
    }
    current = lex_anal(current, token, &token_type);    
    if (strcmp(token, "TABLE") != 0) {
        printf("Error: keyword TABLE expected\n");
        return PARSE_ERROR;
    }
    
    while ( (current != inp + n) && isspace(*current))
        current++;            // Skip space
    current = lex_anal(current, token, &token_type);   // Get table name
    if (token_type != IDENTIFIER) {
        printf("Error: table name expected\n");
        return PARSE_ERROR;
    }
    
    while (isspace(*current)) {
        current++;      // Skip space
    }
    
    if (current != inp + n) {
        printf("Error: too many arguments for table's name, only one expected\n");
        return PARSE_ERROR;
    }
    
    drop_table_name = token;
        
    return NO_ERROR;
}

// function to parse the insert Command
int parseInsert(char *current, char* inp, int n, char* token, int token_type) {
    if (current == inp + n) {
        printf("Error: keyword INTO expected\n");
        return PARSE_ERROR;
    }

        // check the INTO keyword
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "INTO") != 0) {
            printf("Error: keyword INTO expected\n");
            return PARSE_ERROR;
        }
        // check the TABLE keyword
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "TABLE") != 0) {
            printf("Error: keyword TABLE expected\n");
            return PARSE_ERROR;
        }
        
        // check the TABLE table name
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (token_type != IDENTIFIER) {
            printf("Error: table name expected\n");
            return PARSE_ERROR;
        }
        
        // store the table name
        this_table_name = token;
        
        // check the VALUES key word
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "VALUES") != 0) {
            printf("Error: keyword VALUES expected\n");
            return PARSE_ERROR;
        }
        
        // check the "(" key word
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "(") != 0) {
            printf("Error : '(' expected\n");
            return PARSE_ERROR;
        }  
        
        
        int  record_size = 0;
        int  n_fields;           /* Actual # fields */
        int input_size;
        // start to parse the attributes values
        FILE *fp;    /* Used to access the catalog */
        int   fd;    /* Used to access the relation */
        struct DataDescription DataDes[10]; /* Actual # fields */
        n_fields = 0;
        input_size = 0;
        
        fp = fopen("catalog", "r");
        n_fields = 0;
        // record the attributes names and other properties
        while (fscanf(fp, "%s %s %c %d %d", 
            DataDes[n_fields].relname,
            DataDes[n_fields].fieldname,
            &DataDes[n_fields].fieldtype, 
            &DataDes[n_fields].startpos,
            &DataDes[n_fields].fieldsize) > 0 ) {
            if ( strcmp( DataDes[n_fields].relname, this_table_name.c_str()) == 0 ) {
                 if ( DataDes[n_fields].startpos + DataDes[n_fields].fieldsize > record_size ){
                        record_size = DataDes[n_fields].startpos + DataDes[n_fields].fieldsize;
                 }
                 n_fields++;
            }
        }    
        // test whether the table is in the catalog file
        if ( n_fields == 0 ) {
          printf("Relation '%s' not found !\n", this_table_name.c_str());
          return PARSE_ERROR;
        }

        // start to parse the attribute values
        for (int i = 0; i < n_fields; i++) {
            while ( (current != inp + n) && isspace(*current)) {
                current++;      // Skip space
            }
            current = lex_anal(current, token, &token_type);
            if (strcmp(token, ",") == 0) {
                while ( (current != inp + n) && isspace(*current)) 
                    current++;      // Skip space
                current = lex_anal(current, token, &token_type);
            }
            if (strcmp(token, ")") == 0 && i == 0) {
                printf("Error: No input values\n");
                return PARSE_ERROR;
            } else if (strcmp(token, ")") == 0) {
                break;
            }
            
            input_size++;
            // tell the type of token
            if ( DataDes[i].fieldtype == 'I' ) {
                // integer type
                if (token_type == INTEGER_NUMBER) {
                    int in = atoi(token);
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 4);
                } else if (token_type == FLOAT_NUMBER) {
                    int in = (double)(atof(token));
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 4);
                } else {
                     printf("Error: Unexpected data type for No.%d value -> %s (should be Integer), and it cannot be converted\n", i+1, token);
                     return PARSE_ERROR;   
                }
            } else if ( DataDes[i].fieldtype == 'F' ) {
                // double type
                if (token_type == INTEGER_NUMBER) {
                    double in = (double)(atoi(token));
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 8);
                } else if (token_type == FLOAT_NUMBER) {
                    double in = atof(token);
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 8);
                } else {
                     printf("Error: Unexpected data type for No.%d value -> %s (should be Float), and it cannot be converted\n", i+1, token);
                     return PARSE_ERROR;                       
                } 
            } else {
              // char type                  
                if (token_type == STRING) {
                    if (DataDes[i].fieldsize < strlen(token)) {
                        printf("Error: input string %s is too long, the max length should be %d", token, DataDes[i].fieldsize);
                        return PARSE_ERROR;
                    }
                    memcpy(&attribute_data[DataDes[i].startpos], token, strlen(token));
                } else {
                    printf("Error: Unexpected data type for No.%d value -> %s (should be String), and it cannot be converted\n", i+1, token);
                    return PARSE_ERROR;                                       
                }
            } 
        }

        while ( (current != inp + n) && isspace(*current)) 
            current++;      // Skip space
        current = lex_anal(current, token, &token_type);
        
        if (input_size != n_fields ) {
            printf("Error: Unexpected number of input attribute values\nThere should be %d values!\n", n_fields);
            return PARSE_ERROR;
        } else if (strcmp(token, ",") == 0)  {
            printf("Error: Unexpected number of input attribute values\nThere should be %d values!\n", n_fields);
            return PARSE_ERROR;
        } else if (strcmp(token, ")") != 0) {
            printf("Error : ')' expected\n");
            return PARSE_ERROR;
        } else {
            attribute_data[record_size] = 'Y';
            ofstream relation_stream;
            relation_stream.open(this_table_name.c_str(), ios::app);
            for (int count = 0; count < record_size+1; count++) {
                relation_stream << attribute_data[count];
            }
            relation_stream.close();
        }
        
    return NO_ERROR;
}

int parseDelete(char *current, char* inp, int n, char* token, int token_type) {
    struct select_structure *selo;
    selo = (struct select_structure *) malloc(sizeof(struct select_structure));
    int testcount;
    if (current == inp + n) {
        printf("Error: keyword TABLE expected\n");
        return PARSE_ERROR;
    }

        // check the TABLE keyword
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "TABLE") != 0) {
            printf("Error: keyword TABLE expected\n");
            return PARSE_ERROR;
        }
        // check the TABLE table name
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (token_type != IDENTIFIER) {
            printf("Error: table name expected\n");
            return PARSE_ERROR;
        }
        // store the table name
        this_table_name = token;
        
        // check the WHERE key word
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        
        // check whether reach to the end, delete all
        if (current == inp + n) {
            int find_count;
            char *cstr = new char[this_table_name.length() + 1];
            strcpy(cstr, this_table_name.c_str());
            if (FILE *file = fopen(cstr, "r")) {
                fclose(file);
                printf("%s%s\n", "Start to delete all the records in the relation ", cstr);
                // new class table_info
                ScanTable *table_info = new ScanTable();
                table_info->Open(cstr);
                ofstream new_relation_stream;
                string newname = "copy";
                new_relation_stream.open(newname.c_str());
                while ( table_info->GetNext() != 0 ) { 
                    find_count++;
                    table_info->buf[table_info->record_size] = 'N';
                    for (int count = 0; count < table_info->record_size+1; count++) {
                        new_relation_stream << table_info->buf[count];
                    }
                }
                new_relation_stream.close();
                std::remove(this_table_name.c_str());
                std::rename(newname.c_str(), this_table_name.c_str());
                cout << "delete all records..." << endl;
            } else {
                printf("%s\n", cstr);
                cout << "Error: Relation " << this_table_name << " is not in the database..." << endl;
            }
        }
        
        else {
            current = lex_anal(current, token, &token_type);    
            if (strcmp(token, "WHERE") != 0) {
                printf("Error: keyword WHERE expected\n");
                return PARSE_ERROR;
            }
            
            // allocate space for delete command and boolean expression tree
            char *cstr = new char[this_table_name.length() + 1];
            strcpy(cstr, this_table_name.c_str());
            // new class table_info
            ScanTable *table_info = new ScanTable();
            table_info->Open(cstr);
            rel = table_info;
            
            current_location = current;
            current_type = token_type;
            current_token = token;
            // check the WHERE key word
            while ( (current != inp + n) && isspace(*current)) {
                current++;      // Skip space
            }
            current_location = lex_anal(current_location, current_token, &current_type);
            
            int result = 0, testcount = 0;
            MyNODE *b_expr = BE(selo);
            ofstream new_relation_stream;
            string newname = "copy";
            new_relation_stream.open(newname.c_str());
            int previous_del_count = 0;
            while ( rel->GetNext() != 0 ) { 
                if (b_expr == NULL) { 
                    printf("%s\n", "Query failed...");
                    result = 1;
                    break;
                } else {
                    result = Eval(b_expr, selo);
                    if (result == 1) {
                        break;
                    }
                    if (b_expr->ivalue.b) {
                        if (rel->buf[rel->record_size] == 'N') {
                            previous_del_count++;
                        for (int count = 0; count < rel->record_size+1; count++) {
                            new_relation_stream << rel->buf[count];
                        }
                            continue;
                        }
                        rel->buf[rel->record_size] = 'N';
                        for (int count = 0; count < rel->record_size+1; count++) {
                            new_relation_stream << rel->buf[count];
                        }
                        testcount++;
                        
                    } else {
                        for (int count = 0; count < rel->record_size+1; count++) {
                            new_relation_stream << rel->buf[count];
                        }
                    }
                }  
            }
            if (result == 1) {
                new_relation_stream.close();
                std::remove(newname.c_str());
            } else {
                cout << "Previously deleted labeled records: " << previous_del_count << endl;
                cout << "delete " << testcount << " records in this query..." << endl;
                new_relation_stream.close();
                std::remove(this_table_name.c_str());
                std::rename(newname.c_str(), this_table_name.c_str());                
            }
    }
    // return no error info
    return NO_ERROR;   
}

// function to parse the SELECT function
int parseSelect(struct select_structure *selo) {
    MyNODE *be;
    if (sss >= 1) return 1;
    // SELECT STRUCT for this query
    selo->NAttrs = 0;
    selo->NRels = 0;
    selo->in_type = 0;
    selo->in_count = 0;
    if (current_location == current_inp + current_n) {
        printf("Error: keyword '*' OR attributions name are expected...\n");
        return PARSE_ERROR;
    }
    
    char *saved_select;
    
    select_star_case = 0;
    saved_select = current_location;
    int star_mask = 0;
    be = (struct MyNODE *) malloc(sizeof(struct MyNODE));
    
    // start to parse the attributes
    current_location = lex_anal(current_location, current_token, &current_type);
    if (current_type == FROM) {
        printf("Error: attributions name are expected...\n");
        return PARSE_ERROR;
    }
    if (current_location == current_inp + current_n) {
        printf("Error: keyword FROM expected...\n");
        return PARSE_ERROR;
    }

    // start to parse the FROM part first
    // jump the select staff
    while (current_type != FROM) {
        if (current_location == current_inp + current_n) {
            printf("Error: keyword FROM expected...\n");
            return PARSE_ERROR;
        }
        current_location = lex_anal(current_location, current_token, &current_type);
        while ( (current_location != current_inp + current_n) && isspace(*current_location)) {
            current_location++;      // Skip space
        }
    }
    if (current_location == current_inp + current_n) {
        printf("Error: at least one relation name is required...\n");
        return PARSE_ERROR;
    }
    current_location = peek(current_location, current_token, &current_type);
    if (current_type == WHERE) {
        printf("Error: at least one relation name is required...\n");
        return PARSE_ERROR;
    }
    
    if (current_type != IDENTIFIER) {
        printf("Error: %s, %s\n", current_token, "appropriate relation name is required..");
        return PARSE_ERROR;       
    }
    // scan over
    current_location = lex_anal(current_location, current_token, &current_type);

    int where_test = 0;
    
    // construct the relation table part
    while (current_location != current_inp + current_n || (current_type == WHERE || current_type == IDENTIFIER || current_type == ',')) {
        if (current_type == ',' && current_location == current_inp + current_n) {
            printf("Error: relation name in the FROM is not appropriate...the last cannot be ','\n");
            return PARSE_ERROR;                  
        }
        
        if (current_type == ',') {
            // skip the ','
            current_location = lex_anal(current_location, current_token, &current_type);
        } else if (current_type == IDENTIFIER) {
            // check if the relation is in the database
            if (FILE *file = fopen(current_token, "r")) {
                fclose(file);
                
                // copy the relation name
                for (int i = 0; i < strlen(current_token); i++) {
                    selo->RelName[selo->NRels][i] = current_token[i];
                }   
                // bind to the scanTable class
                ScanTable *table = new ScanTable();
                table->Open(current_token);
                selo->Rel[selo->NRels] = table;

                current_location = peek(current_location, current_token, &current_type);
                // copy the alias name
                if (current_type == IDENTIFIER) {
                    selo->rel_aliased[selo->NRels] = 1;
                    for (int i = 0; i < strlen(current_token); i++) {
                        selo->RelName[selo->NRels + 10][i] = current_token[i];
                    }
                    current_location = lex_anal(current_location, current_token, &current_type);
                    current_location = lex_anal(current_location, current_token, &current_type); 
                } else {
                    current_location = lex_anal(current_location, current_token, &current_type);
                }
                // update the SELECT structure
                selo->NRels++;
            } else {
                printf("Error: %s, %s\n", current_token, "realation is not in the database...");
                return PARSE_ERROR;
            }                 
        } else {
            printf("Error: %s, %s\n", current_token, "appropriate relation name is required..");
            return PARSE_ERROR;               
        }

        if (current_type == WHERE) {
            where_test = 1;
            break;
        }
    }
    
    if (current_type != IDENTIFIER && current_type != ',' && current_type != WHERE && current_location != current_inp + current_n) {
        printf("Error: %s, %s\n", current_token, "appropriate relation name is required..");
        return PARSE_ERROR;       
    }
    
    if (current_type == ',') {
        printf("Error: relation name in the FROM is not appropriate...the last cannot be ','\n");
        return PARSE_ERROR;                  
    }
    if (where_test) {
        if (current_location == current_inp + current_n) {
            printf("Error: there are no boolean expressions following keyword WHERE...\n");
            return PARSE_ERROR;            
        }
    } else {
        selo->where = NULL;
    }
    
    // important!!! update the selo->NRels
    selo->NRels--;
    
    // start to parse the where part
    if (!where_test) {
        selo->where = NULL;    
    } else {
        // check the WHERE key word
        while ( (current_location != current_inp + current_n) && isspace(*current_location)) {
            current_location++;      // Skip space
        }
        current_location = lex_anal(current_location, current_token, &current_type);  
        be = BE(selo); 
        selo->where = be;
        if (be == NULL) return PARSE_ERROR;  
    }
   

    // start to parse the select part
    current_location = saved_select;
    current_location = lex_anal(current_location, current_token, &current_type);
    // test the wild card case 
    if (current_type == '*') {
        select_star_case = 1;
        for (int i = 0; i <= selo->NRels; i++) {
            for (int j = 0; j < selo->Rel[i]->n_fields; j++) {
                current_token = selo->Rel[i]->DataDes[j].fieldname;
                current_type = IDENTIFIER; 
                MyNODE *e = E(selo);
                if (e == NULL) return PARSE_ERROR;
                selo->Attr[selo->NAttrs] = e;
                (selo->NAttrs)++;
            }
        }
        
        selo->NAttrs--;
        //printf("%s\n", "------------------------------------------------");
        ProcessSelect(selo->where, selo->NRels, 0, selo);
        return NO_ERROR;
    }
    
    
    string str;
    while (current_type != FROM) {
        str = str + current_token + " ";
        current_location = lex_anal(current_location, current_token, &current_type);
    }

    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());

    char *pch;
    current_location = pch;
    
    pch = strtok(cstr, ",");
    while (pch != NULL) {
        current_location = pch;
        current_location = lex_anal(current_location, current_token, &current_type);
        while (strcmp(" ", current_token) == 0) {
            current_location = lex_anal(current_location, current_token, &current_type);
        }
        MyNODE *e = E(selo);
        
        if (e == NULL) return PARSE_ERROR;
        selo->Attr[selo->NAttrs] = e;
        selo->NAttrs++;
        pch = strtok (NULL, ",");
    }
    selo->NAttrs--;
    
    //printf("%s\n", selo->RelName[0]);
    
    // process the select structure!
    ProcessSelect(selo->where, selo->NRels, 0, selo);

    sss++;
   // return no error
   return 0;    
}

// helper function to process the select command
void ProcessSelect(struct MyNODE *where, int N_Relations, int currRel, struct select_structure *selo) {
    if (currRel <= N_Relations) {
        fd = open(selo->RelName[currRel], O_RDONLY);
        selo->Rel[currRel]->fd = fd;
        while (selo->Rel[currRel]->GetNext() != 0) {
            if (selo->Rel[currRel]->buf[selo->Rel[currRel]->record_size] == 'N') continue;
            ProcessSelect(where, N_Relations, currRel + 1, selo);
        }
    } else {
        if (selo->parent == NULL) {
            // if it is not a inner query           
            if (where != NULL) {
                    int result = Eval(where, selo);
                    if (result == 1) {currRel = N_Relations + 1; return;}
                    if (where->ivalue.b) {
                        /* ================================================
                             Print the SELECT clause
                         ================================================ */
                        for (int i = 0; i <= selo->NAttrs; i++) {
                            Eval(selo->Attr[i], selo);
                            if (selo->Attr[i]->type_tmp == FLOAT) {
                               printf("%10d", (int)selo->Attr[i]->ivalue.f);
                            } else if (selo->Attr[i]->type_tmp == INT ) {
                                printf("%10d", selo->Attr[i]->ivalue.i);
                            } else if (selo->Attr[i]->type_tmp == CHAR ) {
                                printf("%20s", selo->Attr[i]->ivalue.s);
                            } else if (selo->Attr[i]->type == FLOAT_ATTR) {
                                printf("%10d", (int)*((selo->Attr[i]->value).float_attr));
                            } else if (selo->Attr[i]->type == INT_ATTR) {
                                printf("%10d", *((selo->Attr[i]->value).int_attr));
                            } else if (selo->Attr[i]->type == CHAR_ATTR) {
                                printf("%20s", ((selo->Attr[i]->value).char_attr));
                            }
                        }
                    printf("\n");  
                    }             
            } else {
                // if it is an inner query
                for (int i = 0; i <= selo->NAttrs; i++) {
                    Eval(selo->Attr[i], selo);
                    if (selo->Attr[i]->type_tmp == FLOAT) {
                       printf("%10d", (int)selo->Attr[i]->ivalue.f);
                    } else if (selo->Attr[i]->type_tmp == INT ) {
                        printf("%10d", (int)selo->Attr[i]->ivalue.i);
                    } else if (selo->Attr[i]->type_tmp == CHAR ) {
                        printf("%20s", selo->Attr[i]->ivalue.s);
                    } else if (selo->Attr[i]->type_tmp == FLOAT_ATTR) {
                        printf("%10d", (int)*((selo->Attr[i]->value).float_attr));
                    } else if (selo->Attr[i]->type_tmp == INT_ATTR) {
                        printf("%10d", (int)*((selo->Attr[i]->value).int_attr));
                    } else if (selo->Attr[i]->type_tmp == CHAR_ATTR) {
                        printf("%20s", ((selo->Attr[i]->value).char_attr));
                    }
                }
                    printf("\n"); 
            }
        } else {
        // or if it is an inner loop
            if (where != NULL) {
                int result = Eval(where, selo);
                if (result == 1) {currRel = N_Relations + 1; return;}
                if (where->ivalue.b) {
                    for (int i = 0; i <= selo->NAttrs; i++) {
                        if (selo->Attr[i]->type_tmp == FLOAT) {
                            selo->in_type= FLOAT;
                            selo->num_res[selo->in_count++] = (int)selo->Attr[i]->ivalue.f;  
                        } else if (selo->Attr[i]->type_tmp == INT ) {
                            selo->in_type= INT;
                            selo->num_res[selo->in_count++] = (int)selo->Attr[i]->ivalue.i;  
                        } else if (selo->Attr[i]->type_tmp == CHAR ) {
                            selo->in_type = CHAR;
                            strcpy(selo->string_res[selo->in_count++], selo->Attr[i]->ivalue.s);  
                        } else if (selo->Attr[i]->type == FLOAT_ATTR) {
                            selo->in_type= FLOAT;
                            selo->num_res[selo->in_count++] = (int)*((selo->Attr[i]->value).float_attr);               
                        } else if (selo->Attr[i]->type == INT_ATTR) {
                            selo->in_type= INT;
                            selo->num_res[selo->in_count++] = (int)*((selo->Attr[i]->value).int_attr);
                        } else if (selo->Attr[i]->type == CHAR_ATTR) {
                            selo->in_type = CHAR;
                            strcpy(selo->string_res[selo->in_count++], ((selo->Attr[i]->value).char_attr));
                        }
                    }
                } 
            } else {
                // if it is an inner query
                    for (int i = 0; i <= selo->NAttrs; i++) {
                        Eval(selo->Attr[i], selo);
                        if (selo->Attr[i]->type_tmp == FLOAT) {
                            selo->in_type= FLOAT;
                            selo->num_res[selo->in_count++] = (int)selo->Attr[i]->ivalue.f;  
                        } else if (selo->Attr[i]->type_tmp == INT ) {
                            selo->in_type= INT;
                            selo->num_res[selo->in_count++] = (int)selo->Attr[i]->ivalue.i;  
                        } else if (selo->Attr[i]->type_tmp == CHAR ) {
                            selo->in_type = CHAR;
                            strcpy(selo->string_res[selo->in_count++], selo->Attr[i]->ivalue.s);  
                        } else if (selo->Attr[i]->type == FLOAT_ATTR) {
                            selo->in_type= FLOAT;
                            selo->num_res[selo->in_count++] = (int)*((selo->Attr[i]->value).float_attr);               
                        } else if (selo->Attr[i]->type == INT_ATTR) {
                            selo->in_type= INT;
                            selo->num_res[selo->in_count++] = (int)*((selo->Attr[i]->value).int_attr);
                        } else if (selo->Attr[i]->type == CHAR_ATTR) {
                            selo->in_type = CHAR;
                            strcpy(selo->string_res[selo->in_count++], ((selo->Attr[i]->value).char_attr));
                        }
                    }                 
            }              
        }
    }
}        



// function to process the following commands following CREATE
void doCreate() {
    ofstream catalog_stream;
    ofstream relation_stream;
    
    // update the catalog file
    catalog_stream.open("catalog", ios::app);
    int starting_address = 0;
    for (int i = 1; i < record_count; i++) {
        // table name
        catalog_stream <<  this_table_name;
        for (int j = 1; j <= 15 - this_table_name.length(); j++) {
            catalog_stream << " ";
        }
        // attribute name
        catalog_stream <<  attribute_names[i];
        for (int j = 1; j <= 15 - attribute_names[i].length(); j++) {
            catalog_stream << " ";
        }
        // attribute type
        catalog_stream <<  data_types[i];
        for (int j = 1; j <= 7; j++) {
            catalog_stream << " ";
        }
        
        int number = data_length[i];
        int digits = 0; while (number != 0) { number /= 10; digits++;}
        // attribute starting address
        if (data_types[i] == 'C') {
            catalog_stream << starting_address;
            int n = starting_address;
            int d;
            if (n == 0) d = 1;
            else { d = 0; while (n != 0) { n /= 10; d++;} }
            for (int j = 1; j <= 10 - d; j++) {
                catalog_stream << " ";
            }
            starting_address = starting_address + 1 + data_length[i];
        } else if (data_types[i] == 'I') {
            while (starting_address % 4 != 0) {
                starting_address++;
            }
            catalog_stream << starting_address;
            int n = starting_address;
            int d;
            if (n == 0) d = 1;
            else { d = 0; while (n != 0) { n /= 10; d++;} }
            for (int j = 1; j <= 10 - d; j++) {
                catalog_stream << " ";
            }
            starting_address = starting_address + data_length[i];
        } else if (data_types[i] == 'F') {
            while (starting_address % 8 != 0) {
                starting_address++;
            }
            catalog_stream << starting_address;
            int n = starting_address;
            int d;
            if (n == 0) d = 1;
            else { d = 0; while (n != 0) { n /= 10; d++;} }
            for (int j = 1; j <= 10 - d; j++) {
                catalog_stream << " ";
            }
            starting_address = starting_address + data_length[i];
        }
        
        // attribute length
        if (data_types[i] == 'C') {
            catalog_stream <<  data_length[i] + 1;
        } else {
            catalog_stream <<  data_length[i];
        }
        
        catalog_stream << endl;
    }
    catalog_stream.close();
    
    // create the empty relation file
    relation_stream.open(this_table_name.c_str());
    relation_stream.close();
    // update the record_count
    record_count = 1;
    
    cout << "Success! Create table" << " " << this_table_name << " " << endl;
}


// function to process the following commands following DROP
void doDrop() {
    // first check whether the table is in the database
    if (FILE *file = fopen(drop_table_name.c_str(), "r")) {
        fclose(file);
        
        // update the catalog
        ifstream infile("catalog");
        ofstream out_stream;
        out_stream.open("copy");
    
        string line;
        for (string line; getline(infile, line);) {
            string table_name = line.substr(0,drop_table_name.length());
            if (table_name == drop_table_name) {
                continue;
            } else {
                out_stream << line;
                out_stream << endl;
            }
        }
        remove("catalog");
        rename("copy", "catalog");
        
        // remove the table file
        remove(drop_table_name.c_str());
        cout << "Success! Drop table" << " " << drop_table_name << " " << endl;
    } else {
        printf("Error: Table '%s' is not in the database\n", drop_table_name.c_str());
    }
}

// function to process the insert command
void doInsert() {
    cout << "Success! Insert table" << " " << this_table_name << " " << endl;
}

void doDelete() {
    //cout << "Success! Delete table" << " " << this_table_name << " " << endl;
}

void doSelect() {
    
}
/***********************************************************************************************/

/*********************************** Main Function *********************************************/
int main() {  
    char *current;
    char inp[10000]; 
    char token[100];
    int  token_type; 
    int  n;
    
    cout << "Start to type your sql command >> ";
     
   while(1) {
    sss = 0;
      n = read_command( inp );          // Read a command
      current = inp;        // Start parsing at p[0]

      while ( current != inp + n ) { // p did not reach end of line 
        while ( (current != inp + n) && isspace(*current)) {
          current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);   // Get next token
       /* ========================================
              Parse command
        ======================================== */
        if (token_type == CREATE) { 
            Command = COMMAND_CREATE;          /* Use your own constant for CREATE */
            err = parseCreate(current, inp, n, token, token_type);
        } else if (token_type == DROP) { 
            Command = COMMAND_DROP;            /* Use your own constant for DROP */
            err = parseDrop(current, inp, n, token, token_type);
        } else if (token_type == INSERT) { 
            Command = COMMAND_INSERT;            /* Use your own constant for INSERT */
            err = parseInsert(current, inp, n, token, token_type);
        } else if (token_type == DELETE) {      /* Use your own constant for DELETE */
            Command = COMMAND_DELETE;
            err = parseDelete(current, inp, n, token, token_type);
        } else if (token_type == SELECT) {
            struct select_structure *selo;
            selo = (struct select_structure *) malloc(sizeof(struct select_structure));
            current_inp = inp;
            current_location = current;
            current_token = token;
            current_type = token_type;
            current_n = n;
            Command = COMMAND_SELECT;
            err = parseSelect(selo);
        } else { 
            Command = COMMAND_ILLEGAL;         /* Indicate error */
        }
        
        if (err == NO_ERROR) { 
            if ( Command == COMMAND_CREATE) {
                doCreate();
            } else if (Command == COMMAND_DROP) {
                doDrop();
            } else if (Command == COMMAND_INSERT) {
                doInsert();
            } else if (Command == COMMAND_DELETE) {
                doDelete();
            } else if (Command == COMMAND_SELECT) {
                doSelect();
            }
        } else {
                record_count = 1;
        }
        // } else {
        //   switch (err) {
        //     case PARSE_ERROR : 
        //       cout << "Unable to parse the commands, please try again...";
        //       break;
        //     case PROCESS_ERROR : 
        //       cout << "Unable to process the commands, please try again...";
        //       break;
        //   } 
      }
    }
}

/***********************************************************************************************/