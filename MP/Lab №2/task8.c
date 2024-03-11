#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum 
{
	ok,
	invalid_input,
    invalid_params,
    weird_values,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
    file_invalid_data,
	unknown_error
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

int char_to_dec(char n)
{
    if (n >= '0' && n <= '9') return n - '0';
    else if(n >= 'A' && n <= 'Z') return n - 'A' + 10;
    else if(n >= 'a' && n <= 'z') return n - 'a' + 10;
    return -1;
}

char dec_to_char(int base, int n)
{
    if (n >= 0 && n <= 9) return '0' + n;
    else if (n >= 10 && n <= 36) return 'A' - 10 + n;
    return 0;
}

status_code column_addition(int base, char* l, char* r, char** ans)
{
    if (l == NULL || r == NULL || ans == NULL || base < 2 || base > 36) return invalid_params;

    int max_len = 0, min_len = 0, extra_zero = 1;
    for(char* i = l; *i; ++i)
    {
        if (extra_zero) 
        {
            if (*i == '0')
            {
                ++l;
                continue;
            }
            extra_zero = 0;
        }

        if (char_to_dec(*i) == -1) return invalid_input;
        ++max_len;
    }
    extra_zero = 1;
    for(char* i = r; *i; ++i)
    {
        if (extra_zero) 
        {
            if (*i == '0')
            {
                ++r;
                continue;
            }
            extra_zero = 0;
        }
        if (char_to_dec(*i) == -1) return invalid_input;
        ++min_len;
    }
    if (min_len > max_len)
    {
        char* char_buff = l;
        l = r;
        r = char_buff;
        int int_buff = max_len;
        max_len = min_len;
        min_len = int_buff;
    }
    
    *ans = (char*) malloc((max_len+2) * sizeof(char));
    if (*ans == NULL) return malloc_error;
    int dec_sum, add_to_next = 0;
    for (int i = 0; i < min_len; ++i)
    {
        dec_sum = char_to_dec(l[max_len-1-i]) + char_to_dec(r[min_len-1-i]);
        dec_sum += add_to_next;
        add_to_next = 0;
        if (dec_sum >= base)
        {
            add_to_next = 1;
            dec_sum -= base;
        }
        
        (*ans)[max_len - i] = (dec_sum <= 9) ? ('0' + dec_sum) : ('A' + dec_sum - 10);
    }
    for(int i = min_len; i < max_len; ++i)
    {
        dec_sum = char_to_dec(l[max_len-1-i]) + add_to_next;
        add_to_next = 0;
        if (dec_sum >= base)
        {
            add_to_next = 1;
            dec_sum -= base;
        }
        (*ans)[max_len - i] = (dec_sum <= 9) ? ('0' + dec_sum) : ('A' + dec_sum - 10);
    }
    int ans_len = max_len + add_to_next + 1;
    if (add_to_next) 
    {
        (*ans)[0] = '1';
    }
    else (*ans)[0] = '0';
    (*ans)[max_len+1] = '\0';
    return ok;
}

status_code some_base_sum(int base, char** ans, int n,...)
{
    if (ans == NULL || n < 2) return invalid_params;

    va_list arg;
    va_start(arg, n);
    char* buff[2];
    status_code status = column_addition(base, va_arg(arg, char*), va_arg(arg, char*), buff);
    if (status != ok) 
    {
        va_end(arg);
        return status;
    }
    int i = 2;
    for(; i < n; ++i)
    {
        status = column_addition(base, buff[i % 2], va_arg(arg, char*), buff - i % 2 + 1);
        if (status != ok)
        {
            va_end(arg);
            if (i > 2) free(buff[1]);
            free(buff[0]);  
            return status;
        }
    }
    *ans = buff[i % 2];
    if (i > 2) free(buff[1 - i % 2]);
    if ((*ans)[0] == '0' && (*ans)[1] != '\0')
    {
        i = 0;
        do
        {
            ++i;
            (*ans)[i-1] = (*ans)[i];
        } while((*ans)[i-1]);
        buff[0] = (char*) realloc(*ans, sizeof(char)*i);
        if (buff[0] == NULL)
        {
            free(*ans);
            return malloc_error;
        }
    }
    va_end(arg);
    return ok;
}

int main(int argc, char** argv)
{
    status_code status;
    char* ans;
    status = some_base_sum(2, &ans, 3, "1000", "100", "1");
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    printf("%s", ans);
    free(ans);
    return 0;
}