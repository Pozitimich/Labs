#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#include "status_codes.h"

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
    
    printf("%c\n", *str);
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

int geom_mean(double* ans, int n, ...)
{
    if (ans == NULL) return invalid_params;
    *ans = 1;
    va_list str;
    va_start(str, n);

    double num;
    int status;
    for(int i = 0; i < n; ++i)
    {
        status = str_to_double(va_arg(str, char*), &num);
        
        if (status != ok) return status;
        *ans *= pow(num, 1.0/n);
    }
    return ok;
}

int main(int argc, char** argv)
{
    double ans = -1;
    int status = geom_mean(&ans, 3, "4", "4", "8");
    printf("%lf %d", ans, status);
    return status;

}