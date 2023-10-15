#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ll long long
#define ull unsigned long long

enum code_status
{
	ok,
	incorrect_input,
	overflow
};

int square_equation(double eps, double a, double b, double c, double* ans, int* answers_count)
{
	double discriminant = b * b - 4 * a * c;
	if (discriminant < -eps / 2)
	{
		*answers_count = 0;
		return ok;
	}
	if (discriminant < eps / 2)
	{
		*answers_count = 1;
		ans = malloc(sizeof(double));
		ans[0] = -b / (2 * a);
		return ok;
	}

	*answers_count = 2;
	ans = malloc(sizeof(double) * 2);
	ans[0] = (-b + sqrt(discriminant)) / (2 * a);
	ans[1] = (-b - sqrt(discriminant)) / (2 * a);
	return ok;
}

void q_square_equations(double eps, double a, double b, double c)
{
	double ans;
	int answers_count;
	switch (square_equation(eps, a, b, c, &ans, &answers_count))
	{
	case ok:
		printf("a = %lf, b = %lf, c = %lf\n", a, b, c);
		for (int i = 0; i < )
	}
}