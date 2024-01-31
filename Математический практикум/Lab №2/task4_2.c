#include <stdio.h>
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
} status_code;

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

status_code polynomial_val(double x, double* ans, int n, ...)
{
    if (ans == NULL) return invalid_params;

    va_list arg;
    va_start(arg,n);

    *ans = 0;
    for(int i = 0; i <= n; ++i)
    {
        *ans *= x;
        *ans += va_arg(arg, double);
    }
    return ok;
}

int main(int argc, char** argv)
{
    double ans;
    int status = polynomial_val(2, &ans, 3, -1, 2, 5, 8);
    printf(errors[status]);
    printf("%d\n", ans);
    return 0;
}