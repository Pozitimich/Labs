#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

status_code solve_dichotomy(double l, double r, double eps, double (*func)(double), double* ans)
{
    if (func == NULL || ans == NULL || eps <= 0 || r < l) return invalid_params;

    if (func(r) * func(l) > 0) return weird_values;
    if (fabs(func(r)) < eps)
	{
		*ans = r;
		return ok;
	}
	if (fabs(func(l)) < eps)
	{
		*ans = l;
		return ok;
	}

    int increasing = func(r) > 0;
    double mid, val;
    do
    {
        mid = (l + r) / 2;
        val = func(mid);
        if (val * func(r) > 0) r = mid;
        else l = mid;
    } while (fabs(val) > eps);

    *ans = mid;
    return ok;
}

double f1(double x)
{
    return x;
}

double f2(double x)
{
    return 2*x + 3;
}

double f3(double x)
{
    return x*x - 4*x + 3;
}

double f4(double x)
{
    return x*x - 4;
}

int main(int argc, char** argv)
{
    double ans, eps = 0.000001;
    int status = solve_dichotomy(-1, 1, eps, f1, &ans);
    printf("1. ");
    if (status)
    {
        printf(errors[status]);
    }
    else printf("%lf\n", ans);
    printf("2. ");
    status = solve_dichotomy(-2, 0, eps, f2, &ans);
    if (status)
    {
        printf(errors[status]);
    }
    else printf("%lf\n", ans);
    printf("3. ");
    status = solve_dichotomy(-1, 4, eps, f3, &ans);
    if (status)
    {
        printf(errors[status]);
    }
    else printf("%lf\n", ans);
    printf("4. ");
    status = solve_dichotomy(-1, 4, eps, f4, &ans);
    if (status)
    {
        printf(errors[status]);
    }
    else printf("%lf\n", ans);

    return 0;
}