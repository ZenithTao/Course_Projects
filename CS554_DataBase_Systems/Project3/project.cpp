// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO
#include <fcntl.h>
#include <iostream>
#include <fstream>  
#include <string.h>
#include <string>
#include "lexanal.h"
#include "project1.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

using namespace std;

/********************************** Global Variables **********************************************/
int err; // for error indication
int record_count = 1;
string this_table_name; // table name
string drop_table_name; // drop table name
char data_types[100]; // data_types array
int data_length[100]; 
std::string attribute_names[100]; // attribute names string array
char attribute_data[1000];      /* For input data */

// define the command type enum
enum COMMAND_TYPE {
    COMMAND_CREATE = 818,
    COMMAND_DROP, COMMAND_INSERT, COMMAND_ILLEGAL
} ;

enum ERROR_TYPE {
    NO_ERROR = 919,
    PARSE_ERROR, PROCESS_ERROR 
} ;

// meta info struct
typedef struct DataDescription {
   char relname[30];
   char fieldname[30];
   char fieldtype;
   int  startpos;
   int  fieldsize;
} DataDescription;
/********************************** Helper Function **********************************************/

// function to parse the following commands following CREATE
int parseCreate(char *current, char* inp, int n, char* token, int token_type) {
  if (current == inp + n) {
        printf("Error : keyword TABLE expected\n");
        return PARSE_ERROR;
  }
  while ( current != inp + n ) { // has not reached the end of command
    while ( (current != inp + n) && isspace(*current)) {
        current++;      // Skip space
    }
    
    current = lex_anal(current, token, &token_type);   // Get the "table" command

    if (strcmp(token, "TABLE") != 0) {
        printf("Error : keyword TABLE expected\n");
        return PARSE_ERROR;
    }
    
    while ( (current != inp + n) && isspace(*current) )
        current++;            // Skip space
    current = lex_anal(current, token, &token_type);   // Get table name
    if (token_type != IDENTIFIER) {
        printf("Error : table name expected\n");
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
                printf("Error : integer size was expected\n");
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

// function to parse the following commands following DROP
int parseDrop(char *current, char* inp, int n, char* token, int token_type) {
  if (current == inp + n) {
        printf("Error : keyword Table expected\n");
        return PARSE_ERROR;
  }
  
    while ( (current != inp + n) && isspace(*current)) {
        current++;      // Skip space
    }
    current = lex_anal(current, token, &token_type);    
    if (strcmp(token, "TABLE") != 0) {
        printf("Error : keyword TABLE expected\n");
        return PARSE_ERROR;
    }
    
    while ( (current != inp + n) && isspace(*current))
        current++;            // Skip space
    current = lex_anal(current, token, &token_type);   // Get table name
    if (token_type != IDENTIFIER) {
        printf("Error : table name expected\n");
        return PARSE_ERROR;
    }
    
    while (isspace(*current)) {
        current++;      // Skip space
    }
    
    if (current != inp + n) {
        printf("Error : too many arguments for table's name, only one expected\n");
        return PARSE_ERROR;
    }
    
    drop_table_name = token;
        
    return NO_ERROR;
}

// function to parse the insert Command
int parseInsert(char *current, char* inp, int n, char* token, int token_type) {
    if (current == inp + n) {
        printf("Error : keyword INTO expected\n");
        return PARSE_ERROR;
    }

        // check the INTO keyword
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "INTO") != 0) {
            printf("Error : keyword INTO expected\n");
            return PARSE_ERROR;
        }
        // check the TABLE keyword
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (strcmp(token, "TABLE") != 0) {
            printf("Error : keyword TABLE expected\n");
            return PARSE_ERROR;
        }
        
        // check the TABLE table name
        while ( (current != inp + n) && isspace(*current)) {
            current++;      // Skip space
        }
        current = lex_anal(current, token, &token_type);    
        if (token_type != IDENTIFIER) {
            printf("Error : table name expected\n");
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
            printf("Error : keyword VALUES expected\n");
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
                printf("Error : No input values\n");
                return PARSE_ERROR;
            } else if (strcmp(token, ")") == 0) {
                printf("%s\n", "!!!");
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
                     printf("Error : Unexpected data type for No.%d value -> %s (should be Integer), and it cannot be converted\n", i+1, token);
                     return PARSE_ERROR;   
                }
            } else if ( DataDes[i].fieldtype == 'F' ) {
                // float type
                if (token_type == INTEGER_NUMBER) {
                    double in = (double)(atoi(token));
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 8);
                } else if (token_type == FLOAT_NUMBER) {
                    double in = atof(token);
                    memcpy(&attribute_data[DataDes[i].startpos], &in, 8);
                } else {
                     printf("Error : Unexpected data type for No.%d value -> %s (should be Float), and it cannot be converted\n", i+1, token);
                     return PARSE_ERROR;                       
                } 
            } else {
              // char type                  
                if (token_type == STRING) {
                    if (DataDes[i].fieldsize < strlen(token)) {
                        printf("Error : input string %s is too long, the max length should be %d", token, DataDes[i].fieldsize);
                        return PARSE_ERROR;
                    }
                    memcpy(&attribute_data[DataDes[i].startpos], token, strlen(token));
                } else {
                    printf("Error : Unexpected data type for No.%d value -> %s (should be String), and it cannot be converted\n", i+1, token);
                    return PARSE_ERROR;                                       
                }
            } 
        }

        while ( (current != inp + n) && isspace(*current)) 
            current++;      // Skip space
        current = lex_anal(current, token, &token_type);
        
        if (input_size != n_fields ) {
            printf("Error : Unexpected number of input attribute values\nThere should be %d values!\n", n_fields);
            return PARSE_ERROR;
        } else if (strcmp(token, ",") == 0)  {
            printf("Error : Unexpected number of input attribute values\nThere should be %d values!\n", n_fields);
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
        printf("Error : Table '%s' is not in the database\n", drop_table_name.c_str());
    }
}

// function to process the insert command
void doInsert() {
    cout << "Success! Insert table" << " " << this_table_name << " " << endl;
}
/***********************************************************************************************/


int main() {
    int Command;            /* Store what command was parsed */       
    char *current;
    char inp[10000]; 
    char token[100];
    int  token_type; 
    int  n;
    cout << "Start to type your sql command >> ";
     
   while(1) {
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