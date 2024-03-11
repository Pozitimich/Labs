#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ull unsigned long long

#define MAX_INT (unsigned int)(-1) / 2
#define MAX_LONG (unsigned long)(-1) / 2

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

enum flags
{
    xor8_flag,
    xor32_flag,
    mask_flag
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
    while(1)
    {
        if (fread(&c, 1, 1, file) != 1) break;
        result ^= c;
    }
    return result;
}

long file_xor32(FILE* file)
{
    char c = 0;
    char r[4] = { 0,0,0,0 };
    long result = 0;
    int i;
    for (i = 0; i < 4; ++i)
    {
        if (fread(&c, 1, 1, file) != 1) break;
        r[i] ^= c;
    }
    for(; i < 4; ++i)
    {
        r[i] ^= 0;
    }
    result = r[3] + 256 * r[2] + 256 * 256 * r[1] + (long)256 * 256 * 256 * r[0]; // 1, 2^8, 2^16, 2^24
    return result;
}

int hex_to_dec(char* str, long* result)
{
    int base = 16;

    char *ptr = str - 1;

    while (*++ptr)
    {
        *result = *result * base + ((isdigit(*ptr)) ? (*ptr - '0') : (*ptr - 'A' + 10));
    }

    return ok;
}

long file_mask_hex(FILE* file, long mask) 
{
    long counter = 0;
    char c = 0;
    char r[4] = { 0,0,0,0 };
    long sum;
    int f_end = 0;
    int i;
    while (!f_end)
    {
        sum = 0;
        for (i = 0; i < 4; ++i)
        {
            if (fread(&c, 1, 1, file) != 1) 
            {
                f_end = 1;
                break;
            }
            r[i] = c;
        }
        for (; i < 4; ++i)
        {
            r[i] = 0;
        }
        
        for (int j = 0; j < 4; ++j)
        {
            long byte = r[j];
            for (int k = 0; k < 3-j; ++k)
            {
                byte *= 256;
            }
            sum += byte;
        }
        if (mask == sum) ++counter;
    }
    return counter;   
}

int bin_print(long n, int bytes)
{
    printf("%ld\n", n);
    long n_copy = n;
    int bits = bytes * 8;
    char* to_input = calloc(bits, sizeof(char));
    if (to_input == NULL)
    {
        return malloc_error;
    }
    for (int i = bits - 1; i >= 0; --i)
    {
        to_input[i] = n_copy & 1;
        n_copy >>= 1;
    }
    for (int i = 0; i < bits; ++i)
    {
        printf("%d", to_input[i]);
    }
    printf("\n");
    return ok;
}

int main(int argc, char** argv)
{
    long x = 0;
    hex_to_dec1("123A", &x);
    printf("%ld\n", x);

    int flag;
    long dec_arg = 0;
    if (argc == 3 && !strcmp(argv[2], "xor8")) flag = xor8_flag;
    else if (argc == 3 && !strcmp(argv[2], "xor32")) flag = xor32_flag;
    else if (argc == 4 && !strcmp(argv[2], "mask"))
    {
        flag = mask_flag;
        if (hex_to_dec(argv[3], &dec_arg) == incorrect_input)
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
    fclose(f);
    return 0;
}