#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define ull unsigned long long
#define ll long long

enum status_code
{
	ok,
	invalid_input,
	overflow,
	malloc_error,
	unknown_error
};

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Unknown error\n"
};

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

int str_to_double(char* in, double* out)
{
	if (in == NULL || out == NULL)
		return invalid_input;
	
    errno = 0;
    char* end;
    *out = strtod(in, &end);
    if(errno == ERANGE)
        return overflow;

    if (errno != 0 && *out == 0)
        return unknown_error;
    
    if(in == end || *end != '\0')
        return invalid_input;
    
    return ok;

}

int abstract_integral(double eps, double* result, int(*f)(double, double*))
{
	if (result == NULL || f == NULL || eps <= 0)
	{
		return invalid_input;
	}

	double x, y;
	double step;
	ull steps = 2;
	double prev, cur;

	int status = f(0.5, &cur);
	if (status != ok) return status;
	do
	{
		prev = cur;
		cur = 0;
		step = 1.0 / steps;
		x = -0.5*step;
		for (int i = 1; i <= steps; ++i)
		{
			x += step;
			status = f(x, &y);
			if (status != ok) return status;
			cur += y;
		}
		cur *= step;
		steps <<= 1;
		if (steps == 0) return overflow;
	} while (fabs(prev - cur) > eps);
	*result = cur;
	return ok;
}

int a_function(double x, double* y)
{
	if (y == NULL || x <= 0) return invalid_input;
	*y = log(1+x) / x;
	return ok;
}

int b_function(double x, double* y)
{
	if (y == NULL) return invalid_input;
	*y = exp(-x*x * 0.5);
	return ok;
}

int c_function(double x, double* y)
{
	if (y == NULL || (1 - x) <= 0) return invalid_input;
	*y = log(1.0 / (1 - x));
	return ok;
}

int d_function(double x, double* y)
{
	if (y == NULL) return invalid_input;
	*y = pow(x, x);
	return ok;
}

int main (int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: <epsilon>\n");
		return invalid_input;
	}
	
	double eps;
	int status = str_to_double(argv[1], &eps);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}

	double result;
	status = abstract_integral(eps, &result, a_function);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}
	else printf("a. %.15lf\n", result);
	
	status = abstract_integral(eps, &result, b_function);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}
	else printf("b. %.15lf\n", result);
	
	status = abstract_integral(eps, &result, c_function);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}
	else printf("c. %.15lf\n", result);
	
	status = abstract_integral(eps, &result, d_function);
	if (status != ok)
	{
		printf(errors[status]);
		return status;
	}
	else printf("d. %.15lf\n", result);
	return 0;
}