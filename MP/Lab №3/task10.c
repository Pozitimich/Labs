#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum 
{
	OK,
	INVALID_INPUT,
    INVALID_PARAMS,
    WEIRD_VALUES,
	OVERFLOW,
	MALLOC_ERROR,
	FILE_OPENING_ERROR,
	FILE_READING_ERROR,
	FILE_WRITING_ERROR,
    FILE_INVALID_DATA,
	UNKNOWN_ERROR
}status_code;

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
    "Invalid parameters were passed to the function\n",
    "The problem is unsolvable for the given arguments\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open the file\n",
	"Errors occurred while reading the file\n",
	"Errors occurred while writing to the file\n",
    "The contents of the file contradict the condition of the task\n",
	"Unknown error\n"
};

status_code build_trees(FILE* in, FILE* out)
{
    char c;
    int tabs = 0, cnt = 1;

    if (in == NULL || out == NULL) return INVALID_PARAMS;
    while (!feof(in))
    {
        c = fgetc(in);
        while ((c == ',' || c == ' ' || c == '\t') && !feof(in))
        {
            c = fgetc(in);
        }
        if (feof(in)) break;
        fprintf(out, "Tree #%d\n", cnt++);
        while (!feof(in))
        {
            if (c == '(')
            {
                ++tabs;
            }
            else if (c == ')')
            {
                --tabs;
                if (tabs < 0) return FILE_INVALID_DATA;
            }
            else
            {
                if (feof(in) || c == '\n') break;
                if(tabs) fputc('\n', out);
                for(int i = 0; i < tabs; ++i) 
                {
                    fputc('\t', out);
                } 
                fputc(c, out);      
            }
            c = fgetc(in);
            while ((c == ',' || c == ' ' || c == '\t') && !feof(in))
            {
                c = fgetc(in);
            }
        }
        if (tabs != 0) return FILE_INVALID_DATA;
        if (!feof(in)) fputc('\n', out);
    }
    return OK;
}

int main(int argc, char** argv)
{
    FILE* in, *out;
    int status;
    if (argc != 3)
    {
        printf("Usage: <input file> <output file>\n");
        return INVALID_INPUT;
    }

    if(!strcmp(argv[1],argv[2]))
    {
        printf(errors[INVALID_INPUT]);
        return INVALID_INPUT;
    }

    in = fopen(argv[1], "r");
    if (in == NULL) 
    {
        printf(errors[FILE_OPENING_ERROR]);
        return FILE_OPENING_ERROR;
    }

    out = fopen(argv[2], "w");
    if (out == NULL)
    {
        fclose(in);
        return FILE_OPENING_ERROR;
    }
    status = build_trees(in, out);
    printf(errors[status]);
    fclose(in);
    fclose(out);
    if (status)
    {
        out = fopen(argv[2], "w");
        fclose(out);
    }
    return status;
}