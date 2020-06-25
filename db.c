#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255


typedef struct 
{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serialize_row(Row *src, void *des)
{
    memcpy(des + ID_OFFSET, &(src->id), ID_SIZE);
    memcpy(des + USERNAME_OFFSET, &(src->username), EMAIL_SIZE);
    memcpy(des + EMAIL_OFFSET, &(src->email), EMAIL_SIZE);
}

void deserialize_row(Row *src, void *des)
{
    memcpy()
}


typedef struct{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum{
    PREPARE_SUCCESS, PREPARE_UNRECOGNIZES_STATEMENT, PREPARE_SYNTAX_ERROR
}PrepareResult;

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef struct{
    StatementType type;
    Row row_to_insert; // Only used by insert statement
}Statement;


PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
        &(statement->row_to_insert.username), &(statement->row_to_insert.email));
        if (args_assigned < 3)
        {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    else if (strncmp(input_buffer->buffer, "select", 6) == 0)
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZES_STATEMENT; 
}



void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZES_COMMAND
}MetaCommandResult;



MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if (strcmp(input_buffer->buffer, ".exit") == 0)
    {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else{
        return META_COMMAND_UNRECOGNIZES_COMMAND;
    }
}




void execute_statement(Statement *statement)
{
    switch(statement->type)
    {
        case STATEMENT_INSERT:
        {
            printf("This is where we should do an insert\n");
            break;
        }
        case STATEMENT_SELECT:
        {
            printf("This is where we should do a select\n");
            break;
        }
        default:
        break;
    }
}


InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
} 

void print_prompt(){printf("db > ");}

void read_input(InputBuffer* input_buffer){
    ssize_t bytes_read =
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length),
        stdin);

        if(bytes_read <= 0){
            printf("Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        input_buffer->input_length = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = 0;
}



int main(int argc, char* argv[]){
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);
        
        if (input_buffer->buffer[0] == '.')
        {
            switch(do_meta_command(input_buffer))
            {
                case META_COMMAND_SUCCESS:
                {
                    continue;
                }
                case META_COMMAND_UNRECOGNIZES_COMMAND:
                {
                    printf("unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
                }
            
            }
        }

        Statement statement;
        switch(prepare_statement(input_buffer, &statement))
        {
            case PREPARE_SUCCESS:
            {
                break; // switch case break, not while
            }
            case PREPARE_UNRECOGNIZES_STATEMENT:
            {
                printf("unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }
        execute_statement(&statement);
        printf("Executed\n");


    }

}
