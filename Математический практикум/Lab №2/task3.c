#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

typedef struct 
{
    int line, letter;
} place;


typedef struct node
{
    place val;
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
int qu_insert(queue* qu, place c)
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

int str_length(char* str, int* ans)
{
    if (str == NULL || ans == NULL) return invalid_params;
    *ans = 0;
    for(int i = 0; str[i]; ++i) ++(*ans);
}

int make_offset_table(char* pattern, int** ans, int* ans_len)
{
    if (pattern == NULL || ans == NULL || ans_len == NULL) return invalid_params;

    str_length(pattern, ans_len);
    
    *ans = (int*) malloc(*ans_len * sizeof(int));
    if (ans == NULL) return malloc_error;

    int matched = 0;
    for(int i = *ans_len-2; i >= 0; --i)
    {
        (*ans)[i] = *ans_len-1 - i;
        for(int j = i+1; j < *ans_len-1; ++j)
        {
            if (pattern[i] == pattern[j])
            {
                (*ans)[i] = (*ans)[j];
                break;
            }
        }
    }
    (*ans)[*ans_len-1] = *ans_len;
    for(int i = 0; i < *ans_len-1; ++i)
    {
        if (pattern[i] == pattern[*ans_len-1])
        {
            (*ans)[*ans_len-1] = (*ans)[i];
            break;
        }
    }
    return ok;
}

int substr_in_file(FILE* in, char* pattern, int* offset_table, int pat_len, queue* matches)
{
    if (in == NULL || pattern == NULL || matches == NULL) return invalid_params;

    int status;
    int line = 1, line_offset = 0, letter;
    for (int i = 0; i < pat_len; ++i)
    {
        if (pattern[i] == '\n') ++line_offset;
    }

    char* str = malloc(pat_len * sizeof(char)); 
    char c;
    int i = pat_len-1, j = 2*pat_len - 1;
    while(1)
    {
        for(int k = i+1; k <= j; ++k)
        {
            c = fgetc(in);
            if (c == '\n') ++line;
            else if(c == EOF) 
            {   
                free(str);
                return ok;
            }
            if (str[k % pat_len] == '\n') letter = pat_len-1;
            str[k % pat_len] = c;
            ++letter;
        }

        j %= pat_len;
        i = j;

        while(1)
        {
            if (j - i == pat_len)
            {
                place occurence;
                occurence.line = line - line_offset;
                occurence.letter = letter - pat_len + 1;
                i = j;
                status = qu_insert(matches, occurence);
                if (status != ok) return status;
                j += offset_table[pat_len-1];
                break;
            }
            else if (str[(pat_len + i) % pat_len] == pattern[pat_len-1-(j-i)]) --i;
            else
            {
                i = j;
                int k = 0;
                for(; k < pat_len; ++k)
                {
                    if (pattern[k] == str[i])
                    {
                        j += offset_table[k];
                        break;
                    } 
                }
                if (k == pat_len) j += pat_len;
                break;
            }
        }
    }
}

int substr_in_files(char* pattern, int n, ...)
{
    if (pattern == NULL) return invalid_params;

    int *offset_table, pat_len;
    int status = make_offset_table(pattern, &offset_table, &pat_len);
    if (status != ok) return status;

    queue matches;
    status = qu_init(&matches);
    if (status != ok) return status;

    va_list arg;
    va_start(arg, n);
    FILE* in;
    char* path;
    for(int i = 0; i < n; ++i)
    {
        path = va_arg(arg, char*);
        
        in = fopen(path, "r");
        if (in == NULL) 
        {
            printf("File #%d opening error\n", i);
            continue;
        }
        status = substr_in_file(in, pattern, offset_table, pat_len, &matches);
        if (status != ok) 
        {
            printf("File #%d: %s", i, errors[status]);
            continue;
        }   
        else 
        {
            int ans_len = matches.length;
            for (int j = 0; j < ans_len; ++j)
            {
                printf("The substring was found in file #%d: line %d, letter %d\n", i+1, 
                matches.head->val.line, matches.head->val.letter);
                qu_remove_head(&matches);
            }
        }
    }
    free(offset_table);
    qu_free(&matches);

    return ok;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage: <substring>\n");
        return invalid_input;
    }

    int status = substr_in_files(argv[1], 2, "3_1test.txt", "3_2test.txt");
    printf(errors[status]);
    return status;
}