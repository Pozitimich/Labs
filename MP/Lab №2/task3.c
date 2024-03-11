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

typedef struct
{
    place* data;
    int size;
    int cnt;
} place_array;

typedef struct node
{
    place val;
    struct node* next;
} node;

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

int substr_in_file(FILE* in, char* pattern, int* offset_table, int pat_len, place_array* matches)
{
    if (in == NULL || pattern == NULL || matches == NULL) return invalid_params;

    int status;
    int line = 1, line_offset = 0, letter;
    for (int i = 0; i < pat_len; ++i)
    {
        if (pattern[i] == '\n') ++line_offset;
    }

    char* str = malloc(pat_len * sizeof(char)); 
    if(str == NULL) return malloc_error;
    char c;
    int i = pat_len-1, j = 2*pat_len - 1;
    while(1)
    {
        for(int k = i+1; k <= j; ++k)
        {
            c = fgetc(in);
            if (c == '\n') ++line;
            else if(feof(in)) 
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
                i = j;
                if (matches->cnt == matches->size)
                {
                    matches->size *= 2;
                    place* tmp = (place*) realloc(matches->data, sizeof(place)*matches->size);
                    if (tmp == NULL)
                    {
                        return malloc_error;
                    }
                    matches->data = tmp;
                }
                matches->data[matches->cnt].letter = letter - pat_len + 1;
                matches->data[matches->cnt].line = line - line_offset;
                ++(matches->cnt);
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

    place_array matches;
    matches.size = 2;
    matches.cnt = 0;
    matches.data = (place*) malloc(sizeof(place)*matches.size);
    if (matches.data == NULL) 
    {
        free(offset_table);
        return malloc_error;
    }
    
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
            for (int j = 0; j < matches.cnt; ++j)
            {
                printf("The substring was found in file #%d: line %d, letter %d\n", i+1, 
                matches.data[j].line, matches.data[j].letter);
            }
        }
        matches.cnt = 0;
    }
    free(offset_table);
    free(matches.data);
    va_end(arg);

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