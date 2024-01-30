#include <stdio.h>
#include <stdlib.h>

#include "status_codes.h"

int str_to_uint(char* str, unsigned* ans)
{
    if (str == NULL || ans == NULL || str[0] == '-') return invalid_params;

	unsigned res = 0;
	int add;
    for (int i = 0; str[i]; ++i)
    {
        if (str[i] < '0' || str[i] > '9') return invalid_input;
        add = str[i] - '0';
        if (res > (UINT_MAX - add) / 10) return overflow;
        
        res = res * 10 + add;
    }
    *ans = res;
    return ok;
}

int str_len(char* str)
{
    int ans = 0;
    --str;
    while (*(++str)) 
        ++ans;
    return ans;
}

int r_reverse_str(char* str, char** ans)
{
    if (str == NULL || ans == NULL) return invalid_params;

    int len = str_len(str);
    *ans = (char*) malloc((len+1) * sizeof(char));
    if (*ans == NULL) return malloc_error;
    
    (*ans)[len] = '\0';
    for(int i = 0; i < len; ++i)
    {
        (*ans)[len-1-i] = str[i];
    }
    return ok;
}

int u_up_odds(char* str, char** ans)
{
    if (str == NULL || ans == NULL) return invalid_params;

    int len = str_len(str);
    printf("%d\n", len);
    *ans = malloc((len+1) * sizeof(char));
    if (*ans == NULL) return malloc_error;

    for(int i = 0; i < len; ++i)
    {
        if (str[i] >= 'a' && str[i] <= 'z') (*ans)[i] = str[i] + 'A' - 'a';
        else (*ans)[i] = str[i];
    }
    (*ans)[len] = '\0';

    return ok;
}

int n_func(char* str, char** ans)
{
    if (str == NULL || ans == NULL) return invalid_params;

    int len = str_len(str);
    *ans = (char*) malloc((len+1) * sizeof(char));
    if (*ans == NULL) return malloc_error;

    char* buf = (char*) malloc((len) * sizeof(char));
    if (buf == NULL) return malloc_error;

    int numbers = 0, letters = 0, specs = 0;
    for(int i = 0; i < len; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9') 
        {
            (*ans)[numbers] = str[i];
            ++numbers;
        }
        else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) 
        {
            buf[letters] = str[i];
            ++letters;
        }
        else 
        {
            buf[len-specs-1] = str[i];
            ++specs;
        }
    }

    for(int i = 0; i < letters; ++i) (*ans)[numbers + i] = buf[i];
    for(int i = 0; i < specs; ++i) 
        (*ans)[numbers + letters + i] = buf[len - 1 - i];
    (*ans)[len] = '\0';

    free(buf);
    return ok;
}

int c_func(char** str, int len, unsigned seed, char** ans)
{
    if (str == NULL || ans == NULL) return invalid_params;

    char** str_copy = (char**) malloc(len * sizeof(char*));
    if (str_copy == NULL) return malloc_error;

    int char_sum = 0;
    for (int i = 0; i < len; ++i)
    {
        str_copy[i] = str[i];
        for(char* j = str[i]; *j; ++j)
        {
            ++char_sum;
        }
    }
    *ans = (char*) malloc((char_sum+1) * sizeof(char));
    if(*ans == NULL) return malloc_error;
     
    srand(seed);
    int rand_index;
    char* ptr_in, *ptr_out = *ans;
    for (int i = len; i > 0; --i)
    {
        rand_index = rand() % i;
        ptr_in = str_copy[rand_index];
        while(*(ptr_in++)) *(ptr_out++) = *(ptr_in-1);
        str_copy[rand_index] = str_copy[i-1];
    }
    *ptr_out = '\0';
    free(str_copy);
    return ok;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf(errors[invalid_input]);
        return invalid_input;
    }

    if (str_len(argv[1]) != 2 || argv[1][0] != '-')
    {
        printf(errors[invalid_input]);
        return invalid_input;
    }

    if (argv[1][1] != 'c' && argc != 3) 
    {
        printf(errors[invalid_input]);
        return invalid_input;
    }
    int status;
    switch (argv[1][1])
    {
    case 'l':
        printf("%d\n", str_len(argv[2]));
        break;

    case 'r':
    {
        char* ans;
        status = r_reverse_str(argv[2], &ans);
        if (status != ok) break;
        for(char* i = ans; *i; ++i)
        {
            printf("%c", *i);
        }
        printf("\n");
        free(ans);
        break;
    }
    case 'u':
    {
        char* ans;
        status = u_up_odds(argv[2], &ans);
        if (status != ok) break;
        for(char* i = ans; *i; ++i)
        {
            printf("%c", *i);
        }
        printf("\n");
        free(ans);
        break;
    }
    case 'n':
    {
        char* ans;
        status = n_func(argv[2], &ans);
        if (status != ok) break;
        for(char* i = ans; *i; ++i)
        {
            printf("%c", *i);
        }
        printf("\n");
        free(ans);
        break;
    }
    case 'c':
    {
        if (argc < 4) 
        {
            printf(errors[invalid_input]);
            return invalid_input;
        }
        unsigned seed;
        if (argv[2][0] == '-')
        {
            printf(errors[invalid_input]);
            return invalid_input;
        }
        status = str_to_uint(argv[2], &seed);
        if (status != ok) break;
        if (argc < 5)
        {
            printf("only one? ok bro...\n");
        }
        char* ans;
        status = c_func(argv + 3, argc - 3, seed, &ans);
        if (status != ok) break;
        for(char* i = ans; *i; ++i)
        {
            printf("%c", *i);
        }
        printf("\n");
        free(ans);
        break;
    }
    default:
        printf(errors[invalid_input]);
        return invalid_input;
        break;
    }

    printf(errors[status]);
    return status;
}