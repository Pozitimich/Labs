#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ull unsigned long long
#define ll long long

enum code_status
{
    ok,
    incorrect_input,
    overflow,
    malloc_error,
    file_opening_error,
    file_reading_error,
    file_writing_error
};

static const char* errors[] =
{
    "Success\n",
    "Incorrect input\n",
    "Overflow error\n",
    "Memory allocation error\n",
    "Cannot open file\n",
    "Cannot read file correctly\n",
    "Writing to file error\n"
};


int d_exclude_arab_nums(FILE* in, FILE* out)
{
	if (in == NULL || out == NULL) return incorrect_input;

	char c = fgetc(in);
	while (c != EOF)
	{
		if (!isdigit(c))
		{
			fprintf(out, "%c", c);
		}
		c = fgetc(in);
	}

	return ok;
}

int i_latin_in_line(FILE* in, FILE* out)
{
	if (in == NULL || out == NULL) return incorrect_input;

    int k = 0;
    char c = fgetc(in);
    while (c != EOF)
    {
        if (c == '\n')
        {
            fprintf(out, "%d\n", k);
            k = 0;
        }
        else if (isalpha(c))
        {
            ++k;
        }
        c = fgetc(in);
    }
    fprintf(out, "%d \n", k);

    return ok;
}

int s_strange_in_line(FILE* in, FILE* out)
{
    if (in == NULL || out == NULL) return incorrect_input;
    
    int k = 0;
    char c = fgetc(in);
    while (c != EOF)
    {
        if (c == '\n')
        {
            fprintf(out, "%d\n", k);
            k = 0;
        }
        else if (!(isalpha(c) || isdigit(c) || c == ' ' || c == '\t'))
        {
            ++k;
        }
        c = fgetc(in);
    }
    fprintf(out, "%d \n", k);

    return ok;
}

int a_letters_to_16b_ascii(FILE* in, FILE* out)
{
    if (in == NULL || out == NULL) return incorrect_input;
    
    char c = fgetc(in);
    while (c != EOF)
    {
        if (c == '\n')
        {
            fprintf(out, "\n");
        }
        else if (!(isdigit(c)))
        {
            fprintf(out, "%x", c);
        }
        else
        {
            fprintf(out, "%c ", c);
        }
        c = fgetc(in);
    }

    return ok;
}

int add_prefix(char* in, char* prefix, char** out)
{
    if (in == NULL || out == NULL || prefix == NULL)
    {
        return incorrect_input;
    }

    int in_len = strlen(in);
    int prefix_len = strlen(prefix);

    *out = calloc(in_len + prefix_len, sizeof(char));
    int added = 0;
    for (int i = in_len; i >= 0; --i)
    {
        if (in[i] == '/') 
        {
            ++i;
            for (int j = 0; j < i; ++j)
            {
                (*out)[j] = in[j];
            }

            for (int j = 0; j < prefix_len; ++j)
            {
                (*out)[i + j] = prefix[j];
            }
            added = 1;
            break;
        }
        (*out)[i+prefix_len] = in[i];
    }

    if (!added)
    {
        for (int j = 0; j < prefix_len; ++j)
        {
            (*out)[j] = prefix[j];
        }
    }
    return ok;
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4)
    {
        printf(errors[incorrect_input]);
        return 1;
    }

    char* flag = argv[1];
    
    if (!(flag[0] == '-' || flag[0] == '/'))
    {
        printf(errors[incorrect_input]);
        return 1;
    }
    int wrong_input = 0;

    char flag_letter = ' ';
    if (flag[1] != '\0')
    {
        if (flag[2] == '\0')
            flag_letter = flag[1];
        else if (flag[3] == '\0' && flag[1] == 'n')
            flag_letter = flag[2];
        else wrong_input = 1;
    }
    
    if (!((argc == 3 && flag[1] != 'n') || (argc == 4 && flag[1] == 'n')))
    {
        wrong_input = 1;
    }

    if (flag_letter != 'd' && flag_letter != 'i' && flag_letter != 's' && flag_letter != 'a')
    {
        wrong_input = 1;
    }

    if (wrong_input)
    {
        printf(errors[incorrect_input]);
        return 1;
    }

    char* in_name = argv[2];
    if (in_name == NULL)
    {
        printf(errors[malloc_error]);
        return 1;
    }
    char* out_name;

    if (flag[1] == 'n')
    {
        out_name = argv[3];
    }
    else
    {
        add_prefix(in_name, "out_", &out_name);
    }

    FILE* input;
    FILE* output;

    if ((input = fopen(in_name, "r")) == NULL)
    {
        printf(errors[file_opening_error]);
        return 1;
    }
    if ((output = fopen(out_name, "w")) == NULL)
    {
        if (flag_letter == 'n') free(out_name);
        fclose(input);
        printf(errors[file_opening_error]);
        return 1;
    }

    switch (flag_letter)
    {
    case 'd':
        d_exclude_arab_nums(input, output);
        break;
    case 'i':
        i_latin_in_line(input, output);
        break;
    case 's':
        s_strange_in_line(input, output);
        break;
    case 'a':
        a_letters_to_16b_ascii(input, output);
        break;
    }

    if (flag_letter == 'n') free(out_name);
    fclose(input);
    fclose(output);

    return 0;
}
