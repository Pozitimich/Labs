#include <stdio.h>
#include <stdlib.h>
#include <float.h>

enum status_code
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
};

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

int str_to_double(char* str, double* ans)
{
    if (str == NULL || ans == NULL) return invalid_params;
	double res = 0, add;
	int sign = 1;
    int i = 0;
    if (str[0] == '-')
    {
        if (!str[1] || str[1] == '.') return invalid_params;
        ++i;
        sign = -1;
    }
    for (; str[i] != '.'; ++i)
    {   
        if(!str[i]) 
        {   
            *ans = res * sign;
            return ok;
        }
        if (str[i] < '0' || str[i] > '9') return invalid_params;
        add = str[i] - '0';
        if (res > (DBL_MAX - add) / 10) return overflow;
        res = res * 10 + add;
    }
    ++i;
    if (!str[i]) return invalid_params;
    
    double coef = 0.1;
    for (; str[i]; ++i)
    {
        if (str[i] < '0' || str[i] > '9') return invalid_params;
        add = str[i] - '0';
        add *= coef;
        if (res > DBL_MAX - add) return overflow;

        res = res + add;
        if (coef < DBL_MIN * 10) return overflow;
        coef *= 0.1;
    }
    *ans = res * sign;
    return ok;
}

int recur_pow(double base, int exp, double* ans)
{
    if (exp == 1) 
    {
        *ans = base;
        return ok;
    }
    if (DBL_MAX / base < base) return overflow;
    int status = recur_pow(base * base, exp >> 1, ans);
    if (status != ok) return status;
    if (exp % 2) 
    {
        if (DBL_MAX / base < *ans) return overflow;
        *ans *= base;
    }
    return ok;
}

int my_pow(double base, int exp, double* ans)
{
    if (ans == NULL) return invalid_params;

    int status, sign = 0;
    if (exp == 0)
    {
        if (base == 0) return weird_values;
        *ans = 1;
        return ok;
    }
    else if (base == 0)
    {
        *ans = 0;
        return ok;
    }
    else if (exp < 0)
    {
        sign = 1;
        exp = -exp;
    }
    status = recur_pow(base, exp, ans);
    if (status != ok) return status;

    if (sign) *ans = 1 / *ans;

    return ok;
}

int main(int argc, char** argv)
{
    if (argc != 3) 
    {
        printf("Usage: <base> <exponent>\n");
        return invalid_input;
    }

    double base;
    int status = str_to_double(argv[1], &base);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    int exp;
    status = str_to_int(argv[2], &exp);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }

    double ans;
    status = my_pow(base, exp, &ans);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    printf("%lf\n", ans);
    return 0;
}