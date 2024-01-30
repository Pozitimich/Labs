#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define ull unsigned long long
#define ll long long

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
	"Unknown error\n"
};

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

int r_function(FILE* file1, FILE* file2, FILE* out)
{
    if (file1 == NULL || file2 == NULL || out == NULL) return invalid_params;

    FILE* cur_file = file1;
    int completed_files = 0;
    char c, c1;
    while(completed_files != 2)
    {
        c1 = ' ';
        while(1)
        {
            c = fgetc(cur_file);
            if (c == ' ' || c == '\n' || c == '\t')
            {
                if (c1 == ' ') continue;

                fputc(' ', out);
                if (completed_files) continue;
                break;
            }
            else if (c == EOF)
            {
                if (!completed_files && c1 != ' ') fputc(' ', out);
                ++completed_files;
                break;
            }
            fputc(c, out);
            c1 = c;
        }
        cur_file = (cur_file == file1) ? file2 : file1;
    }
    return ok;
}

int a_function(FILE* file1, FILE* out)
{
    if (file1 == NULL || out == NULL) return invalid_params;

    int counter = 1;
    char c = fgetc(file1), c1 = ' ';
    while (c != EOF)
    {
        if (c == ' ' || c == '\n' || c == '\t')
        {
            c = fgetc(file1);
            if (c1 == ' ') continue;
            c1 = ' ';
            fputc(' ', out);
            ++counter;
            continue;
        }
        if (counter % 10 == 0)
        {
            if (isupper(c)) c += 32;

            int i = 0, n = 1;
            for(; n <= (int)c; ++i) n <<= 2;
            char* ascii_code = (char*) malloc(i * sizeof(char));
            if (ascii_code == NULL) return malloc_error;

            itoa((int)c, ascii_code, 4);
            fprintf(out, "%s", ascii_code);
            free(ascii_code);
        }
        else if (counter % 2 == 0)
        {
            if (isupper(c)) c += 32;
            fputc(c, out);
        }
        else if (counter % 5 == 0)
        {
            int i = 0, n = 1;
            for(; n <= (int)c; ++i) n <<= 3;
            char* ascii_code = (char*) malloc(i * sizeof(char));
            if (ascii_code == NULL) return malloc_error;

            itoa((int)c, ascii_code, 8);
            fprintf(out, "%s", ascii_code);
            free(ascii_code);
        }
        else fputc(c, out);
        c1 = c;
        c = fgetc(file1);
    }
    return ok;
}

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        printf("Usage: <flag> <file1> (<file2>) <out_file>\n");
        return invalid_input;
    }

    if (strlen(argv[1]) != 2 || (argv[1][0] != '-' && argv[1][0] != '/')) 
    {
        printf(errors[invalid_input]);
        return invalid_input;
    }
    
    int status;
    switch (argv[1][1])
    {
    case 'r':
    {
        if (argc != 5) return invalid_input;
        FILE* file1 = fopen(argv[2], "r");
        if (file1 == NULL)
        {
            printf(errors[file_opening_error]);
            return file_opening_error;
        }
        FILE* file2 = fopen(argv[3], "r");
        if (file2 == NULL)
        {
            fclose(file1);
            printf(errors[file_opening_error]);
            return file_opening_error;
        }
        FILE* out = fopen(argv[4], "w");
        if (out == NULL)
        {
            fclose(file1);
            fclose(file2);
            printf(errors[file_opening_error]);
            return file_opening_error;
        }
        
        status = r_function(file1, file2, out);
        printf(errors[status]);
        fclose(file1);
        fclose(file2);
        fclose(out);
        break;
    }
    case 'a':
    {
        if (argc != 4) return invalid_input;
        FILE* file1 = fopen(argv[2], "r");
        if (file1 == NULL)
        {
            printf(errors[file_opening_error]);
            return file_opening_error;
        }        
        FILE* out = fopen(argv[3], "w");
        if (out == NULL)
        {
            fclose(file1);
            printf(errors[file_opening_error]);
            return file_opening_error;
        }
        status = a_function(file1, out);
        printf(errors[status]);
        fclose(file1);
        fclose(out);
        break;    
    }
    default:
        printf(errors[invalid_input]);
        return invalid_input;
    }
}