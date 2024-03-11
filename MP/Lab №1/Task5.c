#include <stdio.h>
#include <string.h>
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
	file_opening_error,
	file_reading_error,
	file_writing_error,
	wrong_way_error,
    convergence_error,
	unknown_error
};

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open file\n",
	"Cannot read file correctly\n",
	"Writing to file error\n",
	"Wrong way to file\n",
    "The sequence does not converge",
	"Unknown error\n"
};

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

int str_to_double(char* in, double* out)
{
	if (in == NULL || out == NULL)
		return invalid_input;
	
    errno = 0;
    char* end = NULL;
    *out = strtod(in, &end);
    if(errno == ERANGE)
        return overflow;

    if (errno != 0 && *out == 0)
        return unknown_error;
    
    if(in == end || *end != '\0')
        return invalid_input;
    
    return ok;

}

int a_func(double eps, double x, double* ans)
{
    if (ans == NULL || eps < 0) return invalid_input;
    
    double elem = 1;
    int n = 0;
    *ans = 0;
    do
    {
        *ans += elem;
        ++n;
        elem *= x / n;
    } while (fabs(elem) > eps);

    return ok;
}

int b_func(double eps, double x, double* ans)
{
    if (ans == NULL || eps < 0) return invalid_input;
    
    double elem = 1;
    int n = 0;
    *ans = 0;
    do
    {
        *ans += elem;
        ++n;
		elem *= -x*x / (2.0*n * (2.0*n-1));
    } while (fabs(elem) > eps);

    return ok;
}

int c_func(double eps, double x, double* ans)
{
    if (ans == NULL || eps < 0) return invalid_input;
    
    if (fabs(x) > 1) return convergence_error;
    double elem = 1;
    int n = 0;
    *ans = 0;
    do
    {
        *ans += elem;
        ++n;
		elem *= 9.0 * n*n * x*x / ((3.0*n-1) * (3.0*n-2));
    } while (fabs(elem) > eps);

    return ok;
}

int d_func(double eps, double x, double* ans)
{
    if (ans == NULL || eps < 0) return invalid_input;
    
    if (fabs(x) > 1) return convergence_error;
    double elem = -x*x / 2;
    int n = 1;
    *ans = 0;
    do
    {
        *ans += elem;
        ++n;
		elem *= -(2.0*n - 1) * x*x / (2.0*n);
    } while (fabs(elem) > eps);

    return ok;  
}

void solve_status(int status, double ans)
{
    if (status == ok) printf("%lf", ans);
    else if(status == convergence_error)
    {
        printf("The sequence does not converge, answer is ∞?");
    }
    else printf(errors[status]);
    printf("\n");
    return;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf(errors[invalid_input]);
        return 1;
    }

    double eps;
    if (str_to_double(argv[1], &eps) != ok)
    {
        printf(errors[invalid_input]);
        return 1;
    }

    double x;
    if (str_to_double(argv[2], &x) != ok)
    {
        printf(errors[invalid_input]);
        return 1;
    }

    int status;
    double ans;
    status = a_func(eps, x, &ans);
    printf("a. ");
    solve_status(status, ans);
    status = b_func(eps, x, &ans);
    printf("b. ");
    solve_status(status, ans);
    status = c_func(eps, x, &ans);
    printf("c. ");
    solve_status(status, ans);
    status = d_func(eps, x, &ans);
    printf("d. ");
    solve_status(status, ans);

    return 0;
}