#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define ll long long
#define ull unsigned long long
#define eps 1e-15
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

status_code search_finite_repr(int base, ull mantissa, int power10, int* ans)
{
    if (ans == NULL || base < 2) return invalid_params;

    int power2 = power10, power5 = power10;

    while(power2 && !(mantissa % 2))
    {
        mantissa /= 2;
        --power2;
    }
    
    while(power5 && !(mantissa % 5))
    {
        mantissa /= 5;
        --power5;
    }
    if(power2 && (base % 2) || power5 && (base % 5)) *ans = 0;
    else *ans = 1;
    return ok;
}

status_code poly_search_finite_reprs(int base, int** ans, int n, ...)
{
    if (ans == NULL) return invalid_params;

    va_list arg;
    va_start(arg, n);

    *ans = (int*) malloc(sizeof(int) * n);
    if (*ans == NULL) 
    {
        va_end(arg);
        return malloc_error;
    }

    double eps_opposite = 1, frac;
    ull power10 = 0;
    status_code status;

    while (1.0 / eps_opposite > eps)
    {
        if (ULLONG_MAX / 10 < eps_opposite) return overflow;
        eps_opposite *= 10;
        ++power10;
    }

    for(int i = 0; i < n; ++i)
    {
        frac = va_arg(arg, double);
        if (frac > 1) 
        {
            free(*ans);
            va_end(arg);
            return invalid_input;
        }
        status = search_finite_repr(base, eps_opposite*frac, power10, *ans + i);
        if (status != ok) 
        {
            free(*ans);
            va_end(arg);
            return status;
        }
    }
    va_end(arg);
    return ok;
}

int main(int argc, char** argv)
{
    int* ans;
    int n = 3;
    status_code status = poly_search_finite_reprs(8, &ans, n, 0.125, 0.5, 0.04);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    for(int i = 0; i < n; ++i)
    {
        printf("%d. ", i+1);
        if (ans[i]) printf("yes\n");
        else printf("no\n");
    }
    free(ans);
    return 0;
}