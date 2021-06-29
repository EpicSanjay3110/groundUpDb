#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


using std::basic_istream::getline;
///--------------------------

typedef struct {
    char* buffer;            //variable to store the actual character
    size_t buffer_length;    //variable to store the buffer length allocated
    ssize_t input_length;    //variable to store the length of the input
} InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
} Statement;


InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer)); //allocates the size of InputBuffer in the Main memory
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

/////////////////////////////////////// FUNCTION CALLL TO CLOSE ANY INPUT BUFFER//////////////////////////////
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_metacommand(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else { return META_COMMAND_UNRECOGNIZED_COMMAND; } //returns the message that an unrecogonized command has been entered
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strcmp(input_buffer->buffer, "insert") == 0)
    {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0)
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}


///////////// CODE BLOCK TO SELECT THE CRUD OPERATION TYPE///////////
void execute_statement(Statement* statement) {
    switch (statement->type)
    {
    case STATEMENT_INSERT:      /* constant-expression */
        printf("This is where we would do an  insert .\n");
        break;
    case STATEMENT_SELECT:
        printf("This is where we would try a selecte from. \n");
    default:
        break;
    }
}


void print_prompt() { printf("db> "); }

/////////////////FUNCTION DEFINITION TO READ INPUT BUFFER /////////////

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    //-------Print Error if the Input bytes read are lessed than 0-----------//

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    //--------------- Ignore trailing newline by adding -1 to line before----------------//
    input_buffer->input_length = bytes_read - 1;

    //----------------Fixate the length of the Buffer Read so far-------------//
    input_buffer->buffer[bytes_read - 1] = 0;
}




///////////////////////////THE MAIN ENTRY POINT TO THE FUNCTION///////////////
int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();

        if ((input_buffer->buffer != NULL) && (input_buffer->buffer[0] == '.')) {
            switch (do_metacommand(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case(PREPARE_SUCCESS):
            break;
        case(PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s' .\n", input_buffer->buffer);
            continue;
        }
        execute_statement(&statement);
        printf("Executed. \n");
    }
}




