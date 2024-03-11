#include <stdio.h>
#include <stdlib.h>

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

int str_to_int(char* str, int* ans)
{
    if (str == NULL || ans == NULL) return invalid_params;

	int res = 0;
	int sign = 1, add;
    int i = 0;
    if (str[0] == '-')
    {
        if (str[1] == '\0') return invalid_input;
        ++i;
        sign = -1;
    }
    for (; str[i]; ++i)
    {
        if (str[i] < '0' || str[i] > '9') return invalid_input;
        add = str[i] - '0';
        if (res > (INT_MAX - add) / 10) return overflow;
        
        res = res * 10 + add;
    }
    *ans = res * sign;
    return ok;
}

int str_to_dec(char* str, int* ans, int base)
{
    if (str == NULL || ans == NULL || base < 2) return invalid_params;

	int res = 0;
	int sign = 1, add;
    int i = 0;
    if (str[0] == '-')
    {
        if (str[1] == '\0') return invalid_input;
        ++i;
        sign = -1;
    }
    for (; str[i]; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9') 
        {
            if (str[i] - '0' >= base) return invalid_input;
            add = str[i] - '0';
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            if (str[i] - 'A' + 10 >= base) return invalid_input;
            add = str[i] - 'A' + 10;
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            if (str[i] - 'a' + 10 >= base) return invalid_input;
            add = str[i] - 'a' + 10;
        }
        else return invalid_input;

        if (res > (INT_MAX - add) / base) return overflow;
        
        res = res * base + add;
    }
    *ans = res * sign;
    return ok;
}

int search_max_mod_str(char** strings, int len, int base, int* ans, int* index)
{
    if (strings == NULL || index == NULL || base < 2 || base > 36 || len < 1) return invalid_params;

    int cur, status;
    *ans = 0;
    *index = 0;
    for(int i = 0; i < len; ++i)
    {
        status = str_to_dec(strings[i], &cur, base);
        if (status != ok) return status;
        if (cur > 0) cur *= -1; 
        if (*ans > cur)
        {
            *ans = cur;
            *index = i;
        }
    }
    str_to_dec(strings[*index], ans, base);
    return ok;
}

int dec_to_new_base(int num, int base, char** ans)
{
    if (ans == NULL || base < 2 || base > 36) return invalid_params;

    int ans_len = 2;
    int mask = 1;
    if (num < 0) 
    {
        num *= -1;
        ++ans_len;
    }
    while(mask <= num / base) 
    {
        mask *= base;
        ++ans_len;
    }
    *ans = (char*) malloc(ans_len * sizeof(char));
    if (*ans == NULL) return malloc_error;

    int cur;
    (*ans)[ans_len-1] = '\0';
    for(int i = ans_len-2; i >= 0; --i)
    {
        cur = num % base;
        if (cur < 10) (*ans)[i] = '0' + cur;
        else (*ans)[i] = 'A' + cur - 10;
        num /= base;
    }
    if (ans_len > 2 && (*ans)[0] == '0') (*ans)[0] = '-';
    return ok;
}

int compare_str(char* a, char* b, int* ans)
{
    if (a == NULL || b == NULL || ans == NULL) return invalid_params;

    *ans = 1;
    for(int i = 0; a[i] == b[i]; ++i)
    {
        if (a[i] != b[i]) *ans = 0;
    }
    return ok;
}

int main(int argc, char** argv)
{
    int ans;
    compare_str("Stop", argv[argc-1], &ans);
    if (!ans || argc < 3)
    {
        printf("Usage: <base> <number> <number> ... <number> Stop\n");
        printf(errors[invalid_input]);
        return invalid_input;
    }
    int base;
    int status = str_to_int(argv[1], &base);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    if (base < 2 || base > 36)
    {
        printf(errors[invalid_input]);
        return invalid_input;
    }
    int index;
    status = search_max_mod_str(argv+2, argc-3, base, &ans, &index);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    index += 2;
    char* ans_diff_bases[4];
    for(int i = 0; i < 4; ++i)
    {    
        status = dec_to_new_base(ans, 9*(i+1), &(ans_diff_bases[i]));
        if (status != ok)
        {
            for(int j = 0; j < i; ++j) free(ans_diff_bases[i]);
            printf(errors[status]);
            return status;
        }
    }
    
    printf("%s:", argv[index]);
    for(int i = 0; i < 4; ++i)
    {
        printf(" %s", ans_diff_bases[i]);
        free(ans_diff_bases[i]);
    }
    printf("\n");
    return 0;
}