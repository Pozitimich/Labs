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

void h_to100_multiples(int number)
{
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

void p_is_prime(int* ans, ll number)
{
	*ans = 1;
	if (number == 0 || number == 1)
	{
		*ans = -1;
		return;
	}
	if (number == 2) return;
	if (number < 0 || number % 2 == 0)
	{
		*ans = 0;
		return;
	}

	for (int i = 3; i <= (int)sqrt(number); i += 2)
	{
		if (number % i == 0)
		{
			*ans = 0;
			return;
		}
	}
	return;
}

void s_number_to_digits(char* number)
{
	for (int i = 0; i < strlen(number); ++i)
	{
		printf("%c ", number[i]);
	}
	printf("\n");
	return;
}

int e_degree_tab(int number) 
{
	if (number < 1 || number > 10)
	{
		return incorrect_input;
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

int a_sum_to_n(ull* ans, ll n)
{
	if (n <= 0)
	{
		return incorrect_input;
	}
	*ans = ULLONG_MAX;
	if (*ans / n < (n + 1) / 2)
	{
		return overflow;
	}
	
	*ans = (n + 1) / 2  * n;
	*ans += (n + 1) % 2 * n/2;
	return ok;
}

int f_factorial(ull* ans, ll n)
{
	if (n < 0)
	{
		return incorrect_input;
	}

	*ans = 1;
	if (n == 0) return ok;

	for (ll i = n; i >= 1; --i)
	{
		*ans *= i;
		if (LLONG_MAX / *ans < i - 1)
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
		printf("The amount of arguments is not correct, 3 expected.\n");
		return 1;
	}
	if ((argv[2][0] != '-' && argv[2][0] != '/') ||
		((argv[2][1] != 'h')
		&& (argv[2][1] != 'p')
		&& (argv[2][1] != 's')
		&& (argv[2][1] != 'e')
		&& (argv[2][1] != 'a')
		&& (argv[2][1] != 'f')))
	{
		printf("%c %c\n", argv[2][0], argv[2][1]);
		printf("Incorrect flag\n");
		return 1;
	}

	int n_len = 0;
	for (; n_len < strlen(argv[1]); ++n_len)
	{
		if (!is_digit(argv[1][n_len]))
		{
			if (n_len == 0 && argv[1][0] == '-') continue;
			printf("Incorrect input, integer number expected\n");
			return 1;
			break;
		}
	}

	ull ans;
	switch (argv[2][1])
	{
	case 'h':
		h_to100_multiples(atoi(argv[1]));
		break;
	case 'p':
	{
		int ans;
		if (atoll(argv[1]) == LLONG_MAX || atoll(argv[1]) == LLONG_MIN)
		{
			printf("Too big module of number\n");
			break;
		}
		p_is_prime(&ans, atoll(argv[1]));

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
		s_number_to_digits(argv[1]);
		break;
	case 'e':
		if (atoi(argv[1]) == INT_MAX || atoi(argv[1]) == INT_MIN)
		{
			printf("Too big module of number\n");
			break;
		}

		if (e_degree_tab(atoi(argv[1])) == incorrect_input)
		{
			printf("Incorrect number\n");
		}
		break;
	case 'a':
		if (atoll(argv[1]) == LLONG_MAX || atoll(argv[1]) == LLONG_MIN)
		{
			printf("Too big module of number\n");
			break;
		}
		switch (a_sum_to_n(&ans, atoll(argv[1])))
		{
		case incorrect_input:
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
		if (atoll(argv[1]) == LLONG_MAX || atoll(argv[1]) == LLONG_MIN)
		{
			printf("Too big module of number\n");
			break;
		}
		switch (f_factorial(&ans, atoll(argv[1])))
		{
		case incorrect_input:
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
		break;
	}
	return 1;
}