#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum code_status
{
	ok,
	incorrect_input,
	overflow,
	malloc_error,
};

static const char* errors[] =
{
	"Success\n",
	"Incorrect input\n",
	"Overflow error\n",
	"Memory allocation error\n",
};

void increment(int* n)
{
	int mask = 1;
	while (*n & mask)
	{
		*n ^= mask;
		mask <<= 1;
	}
	*n |= mask;

	return;
}

void decrement(int* n)
{
	int mask = 1;
	while (~(*n) & mask)
	{
		*n |= mask;
		mask <<= 1;
	}
	*n ^= mask;

	return;
}

int to_binbase_system(int n, int r, char** ans, int* length)
{
	if (n == 0)
	{
		*ans = calloc(1, sizeof(char));
		if (*ans == NULL)
		{
			return malloc_error;
		}
		**ans = '0';
	}

	char* pre_ans = calloc(8, sizeof(int));
	if (pre_ans == NULL)
	{
		return malloc_error;
	}

	int negative = 0;
	if (n < 0)
	{
		n ^= -1;
		increment(&n);
		negative = 1;
		printf("n %d\n", n);
	}

	int mask = 1;
	for (int i = 1; i < r; increment(&i))
	{
		mask <<= 1;
		increment(&mask);
	}
	
	for (*length = 0; n != 0; increment(length))
	{
		pre_ans[*length] |= (n & mask);
		n >>= r;
	}
	int len = *length;
	if (negative)
	{
		increment(length);
		*ans = calloc(*length, sizeof(char));
		(*ans)[0] = '-';
	}
	else 
	{
		*ans = calloc(*length, sizeof(char));
	}

	if (*ans == NULL)
	{
		return malloc_error;
	}
	
	decrement(&len);
	{
		int i = len;
		int j = 0;
		while (j != len)
		{
			(*ans)[i] = pre_ans[j];
			decrement(&i);
			increment(&j);
		}
	}
	free(pre_ans);

	return ok;
}

int main(int argc, char** argv)
{
	char** ans = malloc(sizeof(char*));
	if (ans == NULL)
	{
		printf("%s", errors[malloc_error]);
		return 1;
	}
	int* len = malloc(sizeof(int));
	if (len == NULL)
	{
		free(ans);
		printf("%s", errors[malloc_error]);
		return 1;
	}

	to_binbase_system(atoi(argv[1]), atoi(argv[2]), ans, len);
	for (int i = 0; i < *len; increment(&i))
	{
		printf("%ñ", (*ans)[i]);
	}
	printf("\n");

	free(*ans);
	free(ans);
	free(len);
	return 0;
}