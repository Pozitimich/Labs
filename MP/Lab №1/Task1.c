#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ll long long
#define ull unsigned long long

typedef enum
{
	ok,
	invalid_input,
	overflow
} status_code;

void h_to100_multiples(int number)
{
	number = abs(number);
	if (number <= 0 || number > 100)
	{
		printf("There are no numbers from 1 to 100 that are multiples of yours.\n");
		return;
	}

	for (int i = number; i <= 100; i += number)
	{
		printf("%d\n", i);
	}
	return;
}

int p_is_prime(ll number)
{
	if (number <= 1)
	{
		return -1;
	}
	if (number == 2) return 1;
	if (number % 2 == 0)
	{
		return 0;
	}

	for (int i = 3; i * i <= number; i += 2)
	{
		if (number % i == 0)
		{
			return 0;
		}
	}
	return 1;
}

void s_number_to_digits(ll number)
{
	if (number < 0)
	{
		number = -(number);
		printf("- ");
	}
	int len = 0, k = 1;
	while (number / k != 0) 
	{
		k *= 10;
	}
	while(k != 1)
	{
		k /= 10;
		printf("%d ", number / k);
		number %= k;
	}
	printf("\n");
	return;
}

status_code e_degree_tab(int number) 
{
	if (number < 1 || number > 10)
	{
		return invalid_input;
	}
	printf("|\tbase\t|\tpower\t|\tvalue\n");
	ull value = 1;
	for (int i = 1; i <= 10; ++i) 
	{
		for (int j = 1; j <= number; ++j) 
		{
			value *= i;
			printf("|\t%d\t|\t%d\t|\t%llu\n", i, j, value);
		}
		if (i != 10) printf("\n");
		value = 1;
	}
	return ok;
}

status_code a_sum_to_n(ll* ans, ll n)
{
	if (n <= 0)
	{
		return invalid_input;
	}
	*ans = LLONG_MAX;
	if (*ans / n < (n + 1) / 2)
	{
		return overflow;
	}
	
	*ans = (n + 1) / 2  * (n);
	*ans += (n + 1) % 2 * (n)/2;
	return ok;
}

status_code f_factorial(ll* ans, int n)
{
	if (n < 0)
	{
		return invalid_input;
	}

	*ans = 1;
	if (n == 0) return ok;

	for (; n >= 1; --n)
	{
		*ans *= n;
		if (LLONG_MAX / *ans < n - 1)
		{
			return overflow;
		}
	}
	return ok;
}

int is_digit(char symb)
{
	return ('0' <= symb && symb <= '9');
}

int main(int argc, char** argv)
{
	if (argc != 3) 
	{
		printf("Using: <number> <flag>.\n");
		return 1;
	}
	if (argv[2][0] != '-' && argv[2][0] != '/')
	{
		printf("Incorrect flag\n");
		return 1;
	}

	int n_len;
	if (!is_digit(argv[1][0]) && argv[1][0] != '-')
	{
		printf("Incorrect input, integer number expected\n");
		return 1;
	}
	for (n_len = strlen(argv[1]) - 1; n_len > 0; --n_len)
	{
		if (!is_digit(argv[1][n_len]))
		{
			printf("Incorrect input, integer number expected\n");
			return 1;
		}
	}

	ll ll_input = atoll(argv[1]);
	if (ll_input == LLONG_MAX || ll_input == LLONG_MIN)
	{
		printf("Overflow\n");
		return 1;
	}

	ull ans;
	switch (argv[2][1])
	{
	case 'h':
		if (ll_input > INT_MAX || ll_input < INT_MIN)
		{
			printf("Too big number for flag\n");
			return 1;
		}
		h_to100_multiples((int) ll_input);
		break;
	case 'p':
	{
		ans = p_is_prime(ll_input);

		if (ans == 0)
		{
			printf("Number is composite\n");
		}
		else if(ans == 1)
		{
			printf("Number is prime\n");
		}
		else
		{
			printf("Number is not prime or composite");
		}
	}
		break;
	case 's':
		s_number_to_digits(ll_input);
		break;
	case 'e':
		if (ll_input > INT_MAX || ll_input < INT_MIN)
		{
			printf("Too big number for flag\n");
			return 1;
		}
		if (e_degree_tab((int) ll_input) == invalid_input)
		{
			printf("Incorrect number\n");
		}
		break;
	case 'a':
		switch (a_sum_to_n(&ans, ll_input))
		{
		case invalid_input:
			printf("Incorrect input to this function\n");
			break;
		case overflow:
			printf("Too big answer, overflow error\n");
			break;
		case ok:
			printf("%llu\n", ans);
			break;
		default:
			break;
		}
		break;
	case 'f':
		if (ll_input > INT_MAX || ll_input < INT_MIN)
		{
			printf("Too big number for flag\n");
			return 1;
		}
		switch (f_factorial(&ans, (int)ll_input))
		{
		case invalid_input:
			printf("Incorrect input to this function\n");
			break;
		case overflow:
			printf("Too big answer, overflow error\n");
			break;
		case ok:
			printf("%llu\n", ans);
			break;
		default:
			break;
		}
		break;
	default:
		printf("Incorrect flag\n");
		return 1;
		break;
	}
	return 1;
}