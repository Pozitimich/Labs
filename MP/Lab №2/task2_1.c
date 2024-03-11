#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

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

int geom_mean(double* ans, int n, ...)
{
    if (ans == NULL) return invalid_params;
    *ans = 1;
    va_list str;
    va_start(str, n);
    
    double num;
    int sign = 1;
    for(int i = 0; i < n; ++i)
    {
        num = va_arg(str, double);
        if (num < 0)
        {
            num = -num;
            sign = -sign;
        }
        *ans *= pow(num, 1.0/n);
    }
    va_end(str);
    if (sign == -1) 
    {
        if (n % 2 == 0) return weird_values;
        *ans = -*ans;
    }
    return ok;
}

int main(int argc, char** argv)
{
    double ans;
    int status = geom_mean(&ans, 3, 2.0, -4.0, -8.0);
    if (status)
    {
        printf(errors[status]);
    }
    else
    {
        if (ans) printf("Yes\n");
        else printf("No\n");
    }
    return status;

}