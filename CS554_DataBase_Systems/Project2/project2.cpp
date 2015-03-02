// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <iostream>
#include <fstream>  
#include <string.h>
#include <string>
#include "lexanal.h"
#include "project1.h"
#include <ctype.h>

using namespace std;

/********************************** Global Variables **********************************************/
int err; // for error indication
int record_count = 1;
string this_table_name; // table name
string drop_table_name; // drop table name
char data_types[100]; // data_types array
int data_length[100]; 
std::string attribute_names[100]; // attribute names string array

// define the command type enum
enum COMMAND_TYPE {
    COMMAND_CREATE = 818,
    COMMAND_DROP, COMMAND_ILLEGAL
} ;

enum ERROR_TYPE {
    NO_ERROR = 919,
    PARSE_ERROR, PROCESS_ERROR 
} ;
/********************************** Helper Function **********************************************/

// function to parse the following commands following CREATE
int parseCreate(char *current, char* inp, int n, char* token, int token_type) {
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
    while ( current != inp + n ) { // has not reached the end of command
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
        drop_table_name = token;
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
        for (int i = 1; i <= 15 - this_table_name.length(); i++) {
            catalog_stream << " ";
        }
        // attribute name
        catalog_stream <<  attribute_names[i];
        for (int j = 1; j <= 15 - attribute_names[i].length(); j++) {
            catalog_stream << " ";
        }
        // attribute type
        catalog_stream <<  data_types[i];
        for (int i = 1; i <= 7; i++) {
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
    relation_stream.open(this_table_name);
    relation_stream.close();
    // update the record_count
    record_count = 1;
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
    } else {
        printf("Error : Table '%s' is not in the database\n", drop_table_name.c_str());
    }
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
        } else { 
            Command = COMMAND_ILLEGAL;         /* Indicate error */
        }
        
        if (err == NO_ERROR) { 
            if ( Command == COMMAND_CREATE) {
                doCreate();
            } else if (Command == COMMAND_DROP) {
                doDrop();
            } else {
                record_count = 1;
            }
        // } else {
        //   switch (err) {
        //     case PARSE_ERROR : 
        //       cout << "Unable to parse the commands, please try again...";
        //     case PROCESS_ERROR : 
        //       cout << "Unable to process the commands, please try again...";
        //     default:
        //       cout << "Unexpected errors, please try again...";
        //   }
        // }
        }
      }
    }
}