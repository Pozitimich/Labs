#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ull unsigned long long
#define ll long long


enum status_code
{
	OK,
	INVALID_PARAMETER,
	MY_OVERFLOW,
	DIVISION_BY_ZERO,
	UNKNOWN_ERROR,
	RESERVED,
};

//enum code_status
//{
//	ok,
//	overflow,
//	incorrect_input
//};

ull fact(int n)
{
	if (n == 0) return 1;
	ull ans = 1;

	for (int i = n; i > 1; --i)
	{
		ans *= i;
	}
	return ans;
}

double e_limit(double* eps)
{
	int n = 1;
	double ans = 0;
	double prev;
	do
	{
		prev = ans;
		ans = pow(1 + 1.0 / n, n);
		++n;
	} while (ans - prev > *eps);
	return ans;
}

double e_sum(double *eps)
{
	int n = 1;
	double ans = 1;
	ull factorial = 1;
	double prev;
	do
	{
		prev = ans;
		factorial *= n;
		ans += 1.0 / factorial;
		++n;
	} while (ans - prev > *eps);
	return ans;
}

double e_equation(double* eps)
{
	double ans = 0.0;
	double delta = 1.0;
	double prev;
	do
	{
		prev = ans;
		ans += delta;
		if ((log(ans) > 1 && delta > 0) || (log(ans) < 1 && delta < 0)) delta *= -0.5;
	} while (delta*delta > *eps * *eps);
	return ans;
}

double pi_limit(double* eps)
{
	int n = 1;
	double ans = 4.0;
	double prev;
	do
	{
		prev = ans;
		ans *= ((4 * n * n + 4 * n) / pow(2 * n + 1, 2));
		++n;
	} while (prev - ans > *eps);
	return ans;
}

double pi_sum(double* eps)
{
	int n = 1;
	double ans = 0;
	double prev;
	do
	{
		ans += 4.0 / (2 * n - 1);
		prev = ans;
		ans -= 4.0 / (2 * n + 1);
		n += 2;
	} while (prev - ans > *eps);
	return ans;
}

double pi_equation(double* eps)
{
	double ans = 0.0;
	double delta = 1.0;
	double prev;
	do
	{
		prev = ans;
		ans += delta;
		if ((sin(ans) < 0 && delta > 0) || (sin(ans) > 0 && delta < 0)) delta *= -0.5;
	} while (delta * delta > *eps * *eps);
	return ans;
}

double ln2_limit(double* eps)
{
	int n = 1;
	double ans = 0;
	double prev;
	do
	{
		prev = ans;
		ans = n * (pow(2, 1.0 / n) - 1);
		++n;
	} while (fabs(prev - ans) > *eps);
	return ans;
}

double ln2_sum(double* eps)
{
	int n = 1;
	double ans = 0;
	double prev;
	do
	{
		ans += 1.0 / n;
		prev = ans;
		ans -= 1.0 / (n+1);
		n += 2;
	} while (prev - ans > *eps);
	return ans;
}

double ln2_equation(double* eps)
{
	double ans = 0.0;
	double delta = 1.0;
	double prev;
	do
	{
		prev = ans;
		ans += delta;
		if ((exp(ans) > 2 && delta > 0) || (exp(ans) < 2 && delta < 0)) delta *= -0.5;
	} while (delta * delta > *eps * *eps);
	return ans;
}

double sqrt2_limit(double* eps)
{
	double ans = -0.5;
	double prev;
	do 
	{
		prev = ans;
		ans = prev - (prev * prev / 2) + 1;
	} while (fabs(ans - prev) > *eps);
	return ans;
}

double sqrt2_product(double* eps)
{
	int n = 2;
	double ans = 1;
	double prev;
	do
	{
		prev = ans;
		ans *= pow(2, pow(2, -n));
		++n;
	} while (fabs(prev - ans) > *eps);
	return ans;
}

double sqrt2_equation(double* eps)
{
	double ans = 0.0;
	double delta = 1.0;
	double prev;
	do
	{
		prev = ans;
		ans += delta;
		if ((ans * ans > 2 && delta > 0) || (ans * ans < 2 && delta < 0)) delta *= -0.5;
	} while (fabs(delta) > *eps);
	return ans;
}

double gamma_limit(double* eps)
{
	int m = 2;
	long double prev = 0.0, ans = 0.0;
	long double log_fact_k;
	int mult;
	ull combination;
	do
	{
		prev = ans;
		ans = 0.0;
		log_fact_k = 0.0;
		combination = m;
		mult = 1;
		for (int k = 2; k <= m; ++k)
		{
			combination = combination * (m - (k-1)) / k;
			log_fact_k += logl(k);
			ans += (long double)combination * mult / k * log_fact_k;
			mult = -mult;
		}
		++m;
		if (ans < prev) return prev;
	} while (ans - prev > * eps);
	return ans;
}

double gamma_sum(double* eps)
{
	int k = 2;
	double ans = 0;
	double prev;
	int denom = 1, next_denom = 4;
	do
	{
		if (next_denom == k)
		{
			denom = next_denom;
			next_denom = sqrt(next_denom) + 1;
			next_denom *= next_denom;
			++k;
			continue;
		}
		prev = ans;
		ans += 1.0 / denom - 1.0 / k;
		++k;
	} while (fabs(ans - prev) > * eps && k < INT_MAX);

	double pi = acos(-1.0);
	return ans - pi * pi / 6;
}

int is_prime(int number)
{
	if (number == 2) return 1;
	if (number % 2 == 0) return 0;

	for (int i = 3; i <= (int)sqrt(number); i += 2)
	{
		if (number % i == 0)
		{
			return 0;
		}
	}
	return 1;
}

double gamma_equation(double* eps)
{
	if (*eps < 1e-7)
	{
		*eps = 1e-7;
	}
	ull t = 2;
	double ans = 1.0;
	double prev = ans;
	do
	{
		if (!is_prime(t))
		{
			++t;
			continue;
		}
		prev = ans;
		ans = prev * ((double)t - 1.0) / t;
		++t;
	} while (fabs(ans - prev) * log(t) > *eps);
	return -log(ans * log(t));
}

int main(int argc, char** argv)
{
	double eps = 1e-7;

	printf("%.20lf\n", e_limit(&eps));
	printf("%.20lf\n", e_sum(&eps));
	printf("%.20lf\n", e_equation(&eps));
	printf("%.20lf\n", pi_limit(&eps));
	printf("%.20lf\n", pi_sum(&eps));
	printf("%.20lf\n", pi_equation(&eps));
	printf("%.20lf\n", ln2_limit(&eps));
	printf("%.20lf\n", ln2_sum(&eps));
	printf("%.20lf\n", ln2_equation(&eps));
	printf("%.20lf\n", sqrt2_limit(&eps));
	printf("%.20lf\n", sqrt2_product(&eps));
	printf("%.20lf\n", sqrt2_equation(&eps));
	printf("%.20lf\n", gamma_limit(&eps));
	printf("%.20lf\n", gamma_sum(&eps));
	printf("%.20lf\n", gamma_equation(&eps));
}