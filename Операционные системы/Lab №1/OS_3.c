#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ull unsigned long long

enum code_status
{
	ok,
	incorrect_input,
	overflow,
    malloc_error,
    file_opening_error,
    file_reading_error,
    unknown_error
};

static const char* errors[] =
{
    "Success\n",
	"Incorrect input\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open file\n",
    "Cannot read file correctly\n",
    "Unknown error\n"
};

ull file_size(FILE* file) 
{
    fseek(file, 0, SEEK_END);
    ull f_size = ftell(file);
    while (getc(file) != EOF)
    {
        ++f_size;
    }
    rewind(file);
    return f_size;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
        printf("Expected 3 arguments, not %d\n", argc);
		return 1;
	}
    FILE* fin, *fout;

    if ((fin = fopen(argv[1], "rb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        return 1;
    }
    if ((fout = fopen(argv[2], "wb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        fclose(fin);
        return 1;
    }

    ull f_size = file_size(fin);

    char* buff = malloc(sizeof(char) * f_size);
    if (buff == NULL)
    {
        printf("%s", errors[malloc_error]);
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if (fread(buff, sizeof(char), f_size, fin) != f_size)
    {
        printf("%s", errors[file_reading_error]);
        free(buff);
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if (fwrite(buff, sizeof(char), f_size, fout) != f_size)
    {
        printf("%s", errors[unknown_error]);
        free(buff);
        fclose(fin);
        fclose(fout);
        return 1;
    }

    free(buff);
    fclose(fin);
    fclose(fout);

    printf("%s", errors[ok]);
    return 0;
}