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

status_code calculate_polynomial(double x, int coef_num, double* coefs, double* ans)
{
    if (coefs == NULL || ans == NULL) return invalid_params;

    *ans = 0;
    for(int i = coef_num-1; i >= 0; --i)
    {
        *ans *= x;
        *ans += coefs[i];
    }
    return ok;
}

status_code transform_polynomial(double eps, double a, double** ans, int n, ...)
{
    if (ans == NULL) return invalid_params;

    double* coefs = (double*) malloc(n * sizeof(double));
    if (coefs == NULL) return malloc_error;
    *ans = (double*) malloc(n * sizeof(double));
    if (*ans == NULL)
    {
        free(coefs);
        return malloc_error;
    }

    va_list arg;
    va_start(arg, n);
    for(int i = 0; i < n; ++i)
    {
        coefs[i] = va_arg(arg, double);
    }
    va_end(arg);

    status_code status;
    unsigned long long fact = 1;
    for(int i = 0; i < n; ++i)
    {
        status = calculate_polynomial(a, n-i, coefs, *ans + i);
        if (status != ok)
        {
            free(coefs);
            free(*ans);
            return status;
        }

        (*ans)[i] /= fact;
        fact *= i + 1;

        for(int j = 0; j < n-1-i; ++j)
        {
            coefs[j] = (j+1) * coefs[j+1];
        }
        coefs[n-i] = 0;
    }

    free(coefs);
    return ok;
}

int main(int argc, char** argv)
{
	double* coefs;
    int n = 3;
	status_code status = transform_polynomial(1, 4, &coefs, n, 1.0, 3.0, 2.0);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}
	
	for (int i = 0; i < n; ++i)
	{
		printf("g%d = %lf\n", i, coefs[i]);
	}
    return 0;
}