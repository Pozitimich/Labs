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
    return ok;
}

int str_to_dec(char* str, int base, ull* num)
{
    if (str == NULL || num == NULL) return invalid_params;
    *num = 0;
    int cur = 0, sign = 1, i = 0;
    if (str[0] == '-')
    {
        sign = -1;
        ++i;
    }
    for (; str[i]; ++i)
    {
        *num *= base;
        char_to_dec(str[i], &cur);
        *num += cur;
        if (ULLONG_MAX / base < *num) return overflow;
    }
    *num *= sign;
    return ok;
}

int print_str(FILE* out, char* str)
{
    if (str == NULL || out == NULL) return invalid_params;
    if (!str[0]) return ok;

    int skip = 1;
    for(int i = 0; str[i]; ++i)
    {
        if (skip)
        {
            if (str[i] == '0') 
            {
                continue;
            }
            if (str[i] != '-') skip = 0;
        }
        fputc(str[i], out);
    }
    if (skip) fputc('0', out);
    return ok;
}

int handle_tokens(FILE* in, FILE* out)
{
    if (in == NULL || out == NULL) return invalid_params;
    
    int size = 2, cnt = 0;
    char* str = (char*) malloc(sizeof(char) * size);
    if (str == NULL) return malloc_error;

    int cur_base = '1', sign = 1, skip = 1, status;
    ull number;
    char c = fgetc(in);
    while(1)
    {
        if (is_separator(c) || feof(in)) 
        {
            str[cnt] = '\0';
            char_to_dec((char)cur_base, &cur_base);
            ++cur_base;
            status = str_to_dec(str, cur_base, &number);
            if (status != ok)
            {
                free(str);
                return status;
            }
            status = print_str(out, str);
            if (status != ok)
            {
                free(str);
                return status;
            }
            fputc(' ', out);
            fprintf(out, "%d %d\n", cur_base, number);
            cur_base = 2;
            sign = 1;
            
            cnt = 0;
            if (feof(in))
            {
                free(str);
                return ok;
            }
            while (is_separator(c)) c = fgetc(in);
        }
        if (c == '-' && sign == 1) 
        {
            sign = -1;
            skip = 0;
        }
        else if (skip)
        {
            if (c == '0' && cnt > 0)
            {
                c = fgetc(in);
                continue;
            }
            skip = 0;
        }
        else if (isalpha(c)) c = tolower(c);
        else if (!(isdigit(c)))
        {
            free(str);
            return file_invalid_data;
        }

        if (cnt == size-2)
        {
            size *= 2;
            char* tmp = realloc(str, sizeof(char)*size);
            if (tmp == NULL)
            {
                free(str);
                return malloc_error;
            }
            str = tmp;
        }
        str[cnt++] = c;

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