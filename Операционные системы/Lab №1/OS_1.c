#include <stdio.h>
#include <stdlib.h>

enum code_status
{
	ok,
	incorrect_input,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
	wrong_way_error
};

static const char* errors[] =
{
	"Success\n",
	"Incorrect input\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open file\n",
	"Cannot read file correctly\n",
	"Writing to file error\n",
	"Wrong way to file\n"
};
	
int main(int argc, char** argv) 
{
	if (argc != 2)
	{
		printf("Expected 2 arguments\n");
		return 1;
	}

	FILE* f;
	if ((f = fopen(argv[1], "wb")) == NULL)
	{
		printf("%s", errors[wrong_way_error]);
		return 1;
	}

    unsigned char to_write[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    if (fwrite(to_write, sizeof(char), sizeof(to_write) / sizeof(char), f) != sizeof(to_write) / sizeof(char))
    {
        printf("%s", errors[file_writing_error]);
        fclose(f);
        return 1;
    }
	fclose(f);

	if ((f = fopen(argv[1], "rb")) == NULL)
	{
		printf("%s", errors[file_opening_error]);
		return 1;
	}

	unsigned char byte;
    while (fread(&byte, sizeof(char), 1, f))
    {
        printf("%x\n", byte);
        printf("%p\n", f->_Placeholder);
    }
    fclose(f);

    if ((f = fopen(argv[1], "rb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        return 1;
    }

    fseek(f, 3, SEEK_SET);
    
    unsigned char buff[4];
    
    printf("Byte buffer contains %d bytes:\n", fread(buff, sizeof(char), 4, f));
    for (int i = 0; i < 4; ++i)
    {
        printf("%x\n", buff[i]);
    }

    fclose(f);
    return 0;
}