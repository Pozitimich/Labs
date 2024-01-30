#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#include "status_codes.h"

int geom_mean(double* ans, int n, ...)
{
    if (ans == NULL) return invalid_params;
    *ans = 1;
    va_list str;
    va_start(str, n);

    double num;
    for(int i = 0; i < n; ++i)
    {
        num = va_arg(str, double);
        *ans *= pow(num, 1.0/n);
    }
    return ok;
}

int main(int argc, char** argv)
{
    double ans = -1;
    int status = geom_mean(&ans, 3, 2.0, 4.0, 8.0);
    printf("%lf %d", ans);
    return status;

}