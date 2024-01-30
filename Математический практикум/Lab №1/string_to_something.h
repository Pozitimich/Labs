#ifndef string_TO_X
#define string_TO_X

#include "status_codes.h"

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

#endif