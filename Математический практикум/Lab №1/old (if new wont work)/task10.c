#include "my_library.h"

int search_max_str_num(char** strings, int len, int base, int* ans, int* index)
{
    if (strings == NULL || index == NULL || base < 2 || base > 36 || len < 1) return invalid_params;

    int cur;
    int status = str_to_dec(strings[0], ans, base);
    if (status != ok) return status;
    *index = 0;
    for(int i = 1; i < len; ++i)
    {
        status = str_to_dec(strings[i], &cur, base);
        if (status != ok) return status;
        if (*ans < cur)
        {
            *ans = cur;
            *index = i;
        }
    }
    return ok;
}

int dec_to_new_base(int num, int base, char** ans)
{
    if (ans == NULL || base < 2 || base > 36) return invalid_params;

    int ans_len = 2;
    int mask = 1;
    while(mask < num / base) 
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
    compare_str("stop", argv[argc-1], &ans);
    if (!ans || argc < 3)
    {
        printf("Usage: <base> <number> <number> ... <number> stop\n");
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
    int index;
    status = search_max_str_num(argv+2, argc-3, base, &ans, &index);
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