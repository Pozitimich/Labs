#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ull unsigned long long

enum code_status
{
	ok,
	incorrect_input,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	unknown_error
};

static const char* errors[] =
{
	"Success\n",
	"Incorrect input\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open file\n",
	"Cannot read file correctly\n",
	"Unknown error\n"
};

char file_xor8(FILE* file)
{
	char c;
	char result = 0;
	while (1)
	{
		c = getc(file);
		if (c == EOF) break;
		result ^= c;
	}
	return result;
}

long file_xor32(FILE* file)
{
	char c = 0;
	char r[4] = { 0,0,0,0 };
	long result = 0;
	while (c != EOF)
	{
		for (int i = 0; i < 4; ++i)
		{
			c = getc(file);
			if (c == EOF) break;
			r[i] ^= c;
		}
	}
	result = r[3] + 256 * r[2] + 256 * 256 * r[1] + (ull)256 * 256 * 256 * r[0]; // 1, 2^8, 2^16, 2^24
	return result;
}

int hex_to_dec(char* str, int len, long* result)
{
	long pow_16 = 1;
	for (int i = len - 1; i >= 0; --i)
	{
		if ('0' <= str[i] && str[i] <= '9')
		{
			*result += (str[i] - '0') * pow_16;
		}
		else if ('a' <= str[i] && str[i] <= 'f')
		{
			*result += (str[i] - 'a' + 10) * pow_16;
		}
		else if ('A' <= str[i] && str[i] <= 'F')
		{
			*result += (str[i] - 'A' + 10) * pow_16;
		}
		else
		{
			return incorrect_input;
		}
		pow_16 *= 16;
	}
	return ok;
}

long file_mask_hex(FILE* file, long mask)
{
	char c = 0;
	long counter = 0;
	long pow256, to_comp;
	while (c != EOF)
	{
		to_comp = 0;
		pow256 = 1;
		for (int i = 0; i < 4; ++i)
		{
			c = getc(file);
			if (c == EOF) break;
			to_comp += c * pow256;
			pow256 *= 256;
		}
		if (to_comp == mask) ++counter;
	}
	return counter;
}

void bin_print(long n, int bytes)
{
	long n_copy = n;
	char to_input[8];
	int bits = bytes * 8;
	for (int i = bits - 1; i >= 0; --i)
	{
		to_input[i] = n_copy & 1;
		n_copy /= 2;
	}
	for (int i = 0; i < bits; ++i)
	{
		printf("%d", to_input[i]);
	}
	printf("\n");
}

enum flags
{
	xor8_flag,
	xor32_flag,
	mask_flag
};

int main(int argc, char** argv)
{
	int flag;
	long dec_arg = 0;
	if (argc == 3 && !strcmp(argv[2], "xor8")) flag = xor8_flag;
	else if (argc == 3 && !strcmp(argv[2], "xor32")) flag = xor32_flag;
	else if (argc == 4 && !strcmp(argv[2], "mask"))
	{
		flag = mask_flag;
		if (hex_to_dec(argv[3], strlen(argv[3]), &dec_arg) == incorrect_input)
		{
			printf("Incorrect fourth argument\n");
			return 1;
		}
	}
	else
	{
		printf("Incorrect flag or number of arguments\n");
		return 1;
	}

	FILE* f;
	if ((f = fopen(argv[1], "rb")) == NULL)
	{
		printf("%s", errors[file_opening_error]);
		return 1;
	}

	switch (flag)
	{
	case(xor8_flag):
		bin_print(file_xor8(f), sizeof(char));
		break;
	case(xor32_flag):
		bin_print(file_xor32(f), sizeof(long));
		break;
	case(mask_flag):
		printf("%ld\n", file_mask_hex(f, dec_arg));
		break;
	default:
		break;
	}
	return 0;
}