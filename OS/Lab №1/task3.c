#include <stdio.h>
#include <stdlib.h>

#define ull unsigned long long
#define BUF_LEN 1
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


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Expected 3 arguments, not %d\n", argc);
        return 1;
    }
    FILE* fout;
    FILE* fin;

    if ((fout = fopen(argv[1], "rb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        return 1;
    }
    if ((fin = fopen(argv[2], "wb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        fclose(fout);
        return 1;
    }

    char* buff = malloc(BUF_LEN);
    if (buff == NULL)
    {
        printf("%s", errors[malloc_error]);
        fclose(fout);
        fclose(fin);
        return 1;
    }

    size_t len;
    while(1)
    {
        len = fread(buff, 1, BUF_LEN, fout);
        fwrite(buff, 1, len, fin);
        if (len != BUF_LEN) break;
    }

    free(buff);
    fclose(fout);
    fclose(fin);

    int a = printf("%s", errors[ok]);
    printf("%d\n", a);
    return 0;
}