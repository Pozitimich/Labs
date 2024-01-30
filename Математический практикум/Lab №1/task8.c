#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define ull unsigned long long
#define ll long long

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

enum status_code
{
	ok,
	invalid_input,
    invalid_params,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
    file_invalid_data,
	unknown_error
};

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
    "Invalid parameters were passed to the function\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open the file\n",
	"Errors occurred while reading the file\n",
	"Errors occurred while writing to the file\n",
    "The contents of the file contradict the condition of the task\n",
	"Unknown error\n"
};

typedef struct node
{
    char val;
    struct node* next;
} node;

typedef struct my_queue
{
    int length;
    node* head;
    node* tail;
} queue;

int qu_init(queue* qu)
{
    if (qu == NULL) return invalid_params;
    qu->length = 0;
    qu->head = (node*) calloc(1, sizeof(node)); 
    if(qu->head == NULL) return malloc_error;
    qu->tail = qu->head;
    return ok;
}
int qu_insert(queue* qu, char c)
{
    if (qu == NULL) return invalid_params;
    qu->tail->next = (node*) calloc(1, sizeof(node));
    if (qu->tail->next == NULL) return malloc_error;
    qu->tail->val = c;
    qu->tail = qu->tail->next;
    ++(qu->length);
    return ok;
}
int qu_remove_head(queue* qu)
{
    if (qu == NULL || qu->head == NULL) return invalid_params;
    node* new_head = qu->head->next;
    free(qu->head);
    qu->head = new_head;
    --(qu->length);
    return ok;
}
int qu_free(queue* qu)
{
    if (qu == NULL) return invalid_params;
    for(int i = qu->length; i < 0; --i) qu_remove_head(qu);
    free(qu->head);
    return ok;
}

int is_separator(char c)
{
    return (c == ' ' || c == '\n' || c == '\t');
}

int char_to_dec(char c, int* num)
{
    if (num == NULL) return invalid_params;
    if ('A' <= c && c <= 'Z') *num = c - 'A' + 10;
    else if ('a' <= c && c <= 'z') *num = c - 'a' + 10;
    else if ('0' <= c && c <= '9') *num = c - '0';
    else return invalid_params;
}

int queue_to_dec(queue* qu, int base, ull* num)
{
    if (qu == NULL || num == NULL) return invalid_params;
    *num = 0;
    int cur = 0, len = qu->length, sign = 1;
    node* cur_node = qu->head;
    if (cur_node->val == '-')
    {
        cur_node = cur_node->next;
        --len;
        sign = -1;
    }
    for (int i = 0; i < len; ++i)
    {
        *num *= base;
        char_to_dec(cur_node->val, &cur);
        cur_node = cur_node->next;
        *num += cur;
        if (ULLONG_MAX / base < *num) return overflow;
    }
    *num *= sign;
    return ok;
}

int print_queue_number(FILE* out, queue* qu)
{
    if (qu == NULL || out == NULL) return invalid_params;
    if (qu->length == 0) return ok;

    node* node = qu->head;
    int skip = 1;
    for(int i = qu->length; i > 0; --i)
    {
        if (skip)
        {
            if (node->val == '0') 
            {
                node = node->next;
                continue;
            }
            if (node->val != '-') skip = 0;
        }
        fputc(node->val, out);
        node = node->next;
    }
    return ok;
}

int handle_tokens(FILE* in, FILE* out)
{
    if (in == NULL || out == NULL) return invalid_params;
    
    queue qu;
    int status = qu_init(&qu);
    if (status != ok) return status;

    int cur_base = 1, sign = 1;
    ull number;
    char c = fgetc(in);
    while(1)
    {
        if (is_separator(c) || c == EOF) 
        {
            char_to_dec((char)cur_base, &cur_base);
            ++cur_base;
            status = queue_to_dec(&qu, cur_base, &number);
            if (status != ok)
            {
                qu_free(&qu);
                return status;
            }
            status = print_queue_number(out, &qu);
            if (status != ok)
            {
                qu_free(&qu);
                return status;
            }
            fputc(' ', out);
            fprintf(out, "%d %d\n", cur_base, number);
            cur_base = 1;
            sign = 1;
            for (int i = qu.length; i > 0; --i) qu_remove_head(&qu);

            while(is_separator(c)) c = fgetc(in);
            if (c == EOF) 
            {
                qu_free(&qu);
                return ok;
            }
        }

        if (c == '-' && sign == 1) sign = -1;
        else if (isalpha(c)) c = tolower(c);
        else if (!(isdigit(c)))
        {
            qu_free(&qu);
            return file_invalid_data;
        }

        status = qu_insert(&qu, c);
        if (status != ok) 
        {   
            qu_free(&qu);
            return status;
        }
        cur_base = (c > cur_base) ? c : cur_base;

        c = fgetc(in);
    }
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage: <input> <output>\n");
        return invalid_input;
    }
    
    FILE* in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf(errors[file_opening_error]);
        return file_opening_error;
    }
    FILE* out = fopen(argv[2], "w");
    if (out == NULL)
    {
        fclose(in);
        printf(errors[file_opening_error]);
        return file_opening_error;
    }
    
    int status = handle_tokens(in, out);
    printf(errors[status]);

    fclose(in);
    fclose(out);
    return status;
}