#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define FIBONACCI_46 1836311903
#define FIBONACCI_47 2971215073

typedef long long ll;
typedef unsigned long long ull;

int is_base_specifier(char ch)
{
	char specs[18] = { 'c', 's', 'd', 'i', 'o', 'x', 'X', 'u', 'f', 'F', 'e', 'a', 'A', 'E', 'g', 'G', 'n', 'p' };
	for (int i = 0; i < 18; ++i)
	{
		if (ch == specs[i])
		{
			return 1;
		}
	}
	return 0;
}

int is_extra_specifier_begin(char ch)
{
	char specs[6] = { 'R', 'Z', 'C', 't', 'T', 'm' };
	for (int i = 0; i < 6; ++i)
	{
		if (ch == specs[i])
		{
			return 1;
		}
	}
	return 0;
}

void int_to_roman(int number, char* roman)
{	
	if(roman == NULL) return;
	int vals[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
	char* rom_vals[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
	int i = 0;
	if (number < 1 || number > 4000 || roman == NULL) return;
	while (number != 0)
	{
		if (number >= vals[i])
		{
			strcat(roman, rom_vals[i]);
			number -= vals[i];
		}
		else i++;
	}
	return;
}

void uint_to_zeckendorf(unsigned number, char* zeckendorf)
{
	if (zeckendorf == NULL)
	{
		return;
	}
	unsigned cur = FIBONACCI_47;
	unsigned next = FIBONACCI_46;
	int max_ind = 0;
	for (int i = 45; i >= 0; --i)
	{
		if (number >= cur)
		{
			number -= cur;
			zeckendorf[i] = '1';
			max_ind = max_ind ? max_ind : i;
		}
		else
		{
			zeckendorf[i] = '0';
		}
        cur -= next;
		next ^= cur;
        cur ^= next;
        next ^= cur;
	}
	zeckendorf[max_ind + 1] = '1';
	zeckendorf[max_ind + 2] = '\0';
}

int ctoi(char ch)
{
	if (isdigit(ch))
	{
		return ch - '0';
	}
	if (isalpha(ch))
	{
		return tolower(ch) - 'a' + 10;
	}
	return -1;
}

char itoc(int number, int uppercase)
{
	if (number >= 0 && number < 10)
	{
		return '0' + number;
	}
	if (number >= 10 && number < 36)
	{
		char ch = uppercase ? 'A' : 'a';
		return ch + number - 10;
	}
	return '\0';
}

void int_to_str_int(int number, int base, int uppercase, char* str_int)
{
	int iter = 0;
	char tmp;
	if (str_int == NULL)
	{
		return;
	}
	if (base < 2 || base > 36)
	{
		base = 10;
	}
	while (number != 0)
	{
		str_int[iter++] = itoc(abs(number % base), uppercase);
		if (number == -1)
		{
			str_int[iter++] = '-';
		}
		number /= base;
	}
	str_int[iter] = '\0';
	for (int i = 0; 2*i < iter-1; ++i)
	{
		tmp = str_int[i];
		str_int[i] = str_int[iter-i-1];
		str_int[iter-i-1] = tmp;
	}
    return;
}

int str_int_to_int(const char* str_int, int base, int uppercase, int* number)
{
	ull i;
	char* ptr;
	if (str_int == NULL)
	{
		return -1;
	}
	if (base < 2 || base > 36)
	{
		base = 10;
	}
	uppercase = uppercase ? 1 : 0;
	for (i = 0; str_int[i]; ++i)
	{
		if (isalpha(str_int[i]) && isupper(str_int[i]) != uppercase)
		{
			return -1;
		}
	}
	*number = strtoll(str_int, &ptr, base);
	if (*ptr != '\0')
	{
		return -1;
	}
	return 0;
}

void number_dump(void *num, size_t size, char* ans) 
{
	char* ptr = (char*)num;
    int ind;
	if (ans == NULL) return;
	ind = (size-1)*8;
	
    for (int i = size*8; i >= 0; --i)
	{
		ans[ind++] = (((*ptr) >> ((i-1) % 8)) & 1) + '0';
        if ((i-1) % 8 == 0) 
		{
			ind -= 16;
			++ptr;
		}
    }
    ans[size*8] = '\0';
    return;
}

int overfprintf(FILE* file, const char* format, ...)
{
	ull spec_len = 1;
	ull spec_size = 4;
	char* spec;
	int print_cnt;
	va_list va;
	int spec_flag;
	const char* frm_ptr;
	char* tmp;
	char last_ch;
	int cnt, number;
	double dnumber;
	float fnumber;
	char str[65];
	int base, uppercase;
	ull data, i;
	char bit;
	if (file == NULL || format == NULL)
	{
		return -1;
	}

	spec = (char*) malloc(sizeof(char) * 4);
	if (spec == NULL)
	{
		return -1;
	}
	spec[0] = '%';
	
	print_cnt = 0;
	va_start(va, format);
	
	spec_flag = 0;
	frm_ptr = format;
	while (*frm_ptr)
	{
		if (!spec_flag)
		{
			if (*frm_ptr == '%')
			{
				++frm_ptr;
				spec_flag = 1;
				if (*frm_ptr == '%')
				{
					spec_flag = 0;
					putc(*frm_ptr++, file);
					++print_cnt;
				}
			}
			else
			{
				putc(*frm_ptr++, file);
				++print_cnt;
			}
		}
		else
		{
			if (spec_len + 2 == spec_size)
			{
				spec_size *= 2;
				tmp = (char*) realloc(spec, sizeof(char) * spec_size);
				if (tmp == NULL)
				{
					free(spec);
					va_end(va);
					return -1;
				}
				spec = tmp;
			}
			spec[spec_len++] = *frm_ptr++;
			spec[spec_len] = '\0';
			last_ch = spec[spec_len - 1];
			if (last_ch == 'n' && !strcmp(spec, "%n"))
			{
				print_cnt += fprintf(file, "%lld", print_cnt);
				spec_len = 1;
				spec_flag = 0;
			}
			else if (is_base_specifier(last_ch) && last_ch != 'n')
			{
				cnt = 0;
				if (last_ch == 'c' || last_ch == 'd' || last_ch == 'o' || last_ch == 'i' ||
						last_ch == 'x' || last_ch == 'X' || last_ch == 'u')
				{
					cnt += fprintf(file, spec, va_arg(va, long long));
				}
				else if (last_ch == 'f' || last_ch == 'F' || last_ch == 'e' ||
						last_ch == 'a' || last_ch == 'A' || last_ch == 'E' ||
						last_ch == 'g' || last_ch == 'G')
				{
					cnt += fprintf(file, spec, va_arg(va, double));
				}
				else if (last_ch == 'p' || last_ch == 's')
				{
					cnt += fprintf(file, spec, va_arg(va, void*));
				}
				if (cnt == -1)
				{
					va_end(va);
					free(spec);
					return -1;
				}
				print_cnt += cnt;
				spec_len = 1;
				spec_flag = 0;
			}
			else if (is_extra_specifier_begin(last_ch))
			{
				spec[spec_len++] = *frm_ptr++;
				spec[spec_len] = '\0';
				str[0] = '\0';
				if (!strcmp(spec, "%Ro"))
				{
					number = va_arg(va, int);
					int_to_roman(number, str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%Zr"))
				{
					number = (int) va_arg(va, unsigned);
					uint_to_zeckendorf(number, str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%Cv") || !strcmp(spec, "%CV"))
				{
					number = va_arg(va, int);
					base = va_arg(va, int);
					uppercase = *frm_ptr == 'V';
					int_to_str_int(number, base, uppercase, str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%to") || !strcmp(spec, "%TO"))
				{
					tmp = va_arg(va, char*);
					base = va_arg(va, int);
					uppercase = *(frm_ptr-1) == 'O';
					if (str_int_to_int(tmp, base, uppercase, &number))
					{
						return -1;
					}
					print_cnt += fprintf(file, "%d", number);
				}
				else if (!strcmp(spec, "%mi"))
				{
					number = va_arg(va, int);
					number_dump(&number, sizeof(int), str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%mu"))
				{
					number = (int) va_arg(va, int);
					number_dump(&number, sizeof(int), str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%md"))
				{
					dnumber = va_arg(va, double);
					number_dump(&dnumber, sizeof(double), str);
					print_cnt += fprintf(file, "%s", str);
				}
				else if (!strcmp(spec, "%mf"))
				{
					fnumber = (float) va_arg(va, double);
					number_dump(&fnumber, sizeof(float), str);
					print_cnt += fprintf(file, "%s", str);
				}
				else
				{
					va_end(va);
					free(spec);
					return -1;
				}
				spec_len = 1;
				spec_flag = 0;
			}
			else if (isalpha(last_ch) && last_ch != 'l' && last_ch != 'h')
			{
				va_end(va);
				free(spec);
				return -1;
			}
		}
	}
	va_end(va);
	free(spec);
	if (spec_len != 1)
	{
		return -1;
	}
	return print_cnt;
}

int oversprintf(char* dest, const char* format, ...)
{
	ull spec_len;
	ull spec_size;
	char* spec;
	int print_cnt;
	ull dest_len;
	va_list va;
	int spec_flag;
	const char* frm_ptr;
	char* tmp;
	char last_ch;
	int cnt, number;
	double dnumber;
	float fnumber;
	char str[65];
	int base, uppercase;
	ull i;
	size_t data;
	char bit;
	if (dest == NULL || format == NULL)
	{
		return -1;
	}
	
	spec_len = 1;
	spec_size = 4;
	spec = (char*) malloc(sizeof(char) * 4);
	if (spec == NULL)
	{
		return -1;
	}
	spec[0] = '%';
	
	dest_len = 0;
	va_start(va, format);
	
	spec_flag = 0;
	frm_ptr = format;
	while (*frm_ptr)
	{
		if (!spec_flag)
		{
			if (*frm_ptr == '%')
			{
				++frm_ptr;
				spec_flag = 1;
				if (*frm_ptr == '%')
				{
					spec_flag = 0;
					dest[dest_len++] = *frm_ptr++;
				}
			}
			else
			{
				dest[dest_len++] = *frm_ptr++;
			}
		}
		else
		{
			if (spec_len + 2 == spec_size)
			{
				spec_size *= 2;
				tmp = (char*) realloc(spec, sizeof(char) * spec_size);
				if (tmp == NULL)
				{
					free(spec);
					va_end(va);
					return -1;
				}
				spec = tmp;
			}
			spec[spec_len++] = *frm_ptr++;
			last_ch = spec[spec_len - 1];
			if (last_ch == 'n' && !strcmp(spec, "%n"))
			{
				dest_len += sprintf(dest + dest_len, "%lld", dest_len);
				spec_len = 1;
				spec_flag = 0;
			}
			else if (is_base_specifier(last_ch))
			{
				spec[spec_len] = '\0';
				cnt = 0;
				if (last_ch == 'c' || last_ch == 'd' || last_ch == 'o' || last_ch == 'i' ||
						last_ch == 'x' || last_ch == 'X' || last_ch == 'u')
				{
					cnt += sprintf(dest + dest_len, spec, va_arg(va, long long));
				}
				else if (last_ch == 'f' || last_ch == 'F' || last_ch == 'e' ||
						last_ch == 'a' || last_ch == 'A' || last_ch == 'E' ||
						last_ch == 'g' || last_ch == 'G')
				{
					cnt += sprintf(dest + dest_len, spec, va_arg(va, double));
				}
				else if (last_ch == 'p' || last_ch == 's')
				{
					cnt += sprintf(dest + dest_len, spec, va_arg(va, void*));
				}
				if (cnt == -1)
				{
					va_end(va);
					free(spec);
					return -1;
				}
				dest_len += cnt;
				spec_len = 1;
				spec_flag = 0;
			}
			else if (is_extra_specifier_begin(last_ch))
			{
				spec[spec_len++] = *frm_ptr++;
				spec[spec_len] = '\0';
				str[0] = '\0';
				if (!strcmp(spec, "%Ro"))
				{
					number = va_arg(va, int);
					int_to_roman(number, str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else if (!strcmp(spec, "%Zr"))
				{
					number = (int) va_arg(va, unsigned);
					uint_to_zeckendorf(number, str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else if (!strcmp(spec, "%Cv") || !strcmp(spec, "%CV"))
				{
					number = va_arg(va, int);
					base = va_arg(va, int);
					uppercase = *frm_ptr == 'V';
					int_to_str_int(number, base, uppercase, str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else if (!strcmp(spec, "%to") || !strcmp(spec, "%TO"))
				{
					tmp = va_arg(va, char*);
					base = va_arg(va, int);
					uppercase = *(frm_ptr-1) == 'O';
					if (str_int_to_int(tmp, base, uppercase, &number))
					{
						return -1;
					}
					dest_len += sprintf(dest + dest_len, "%d", number);
				}
				else if (!strcmp(spec, "%mi") || !strcmp(spec, "%mu"))
				{
					number = va_arg(va, int);
					number_dump(&number, sizeof(int), str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else if (!strcmp(spec, "%md"))
				{
					dnumber = va_arg(va, double);
					number_dump(&dnumber, sizeof(double), str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else if (!strcmp(spec, "%mf"))
				{
					fnumber = (float) va_arg(va, double);
					number_dump(&fnumber, sizeof(float), str);
					dest_len += sprintf(dest + dest_len, "%s", str);
				}
				else
				{
					va_end(va);
					free(spec);
					return -1;
				}
				spec_len = 1;
				spec_flag = 0;
			}
			else if (isalpha(last_ch) && last_ch != 'l' && last_ch != 'h')
			{
				va_end(va);
				free(spec);
				return -1;
			}
		}
	}
	dest[dest_len] = '\0';
	va_end(va);
	free(spec);
	if (spec_len != 1)
	{
		return -1;
	}
	return dest_len;
}

int main(int argc, char** argv)
{
	int x = 0;
	char dest[512];
	x = overfprintf(stdout, "%lld %c %s %n %E\n", 5, 'q', "str", 123.456);
	x = overfprintf(stdout, "\"%Ro\" %Ro %Ro %Ro\n", 0, 4, 99, 3888);
	x = overfprintf(stdout, "%Zr %Zr %Zr %Zr %Zr %Zr\n", 0, 1, 2, 3, 4, UINT_MAX);
	x = overfprintf(stdout, "%Cv %CV %Cv\n", INT_MAX, 2,  INT_MIN, 2,  INT_MIN+1, 2);
	x = overfprintf(stdout, "%Cv %CV %Cv\n", 255, 16,  107, 36,  15, 4);
	x = overfprintf(stdout, "%to %TO %to\n", "1y", 36,  "FF", 16,  "33", 4);
	x = overfprintf(stdout, "%mi\n", 10);
	x = overfprintf(stdout, "%mu\n", 10);
	x = overfprintf(stdout, "%md\n", -312.3125);
	x = overfprintf(stdout, "%mf\n", -312.3125);
	printf("%d\n\n", x);
	
	x = oversprintf(dest, "%lld %c %s %n %E\n", 5, 'q', "str", 123.456);
	printf("%s", dest);
	x = oversprintf(dest, "\"%Ro\" %Ro %Ro %Ro\n", 0, 4, 99, 3888);
	printf("%s", dest);
	x = oversprintf(dest, "%Zr %Zr %Zr %Zr %Zr %Zr\n", 0, 1, 2, 3, 4, UINT_MAX);
	printf("%s", dest);
	x = oversprintf(dest, "%Cv %CV %Cv\n", INT_MAX, 2,  INT_MIN, 2,  INT_MIN+1, 2);
	printf("%s", dest);
	x = oversprintf(dest, "%Cv %CV %Cv\n", 255, 16,  107, 36,  15, 4);
	printf("%s", dest);
	x = oversprintf(dest, "%to %TO %to\n", "1y", 36,  "FF", 16,  "33", 4);
	printf("%s", dest);
	x = oversprintf(dest, "%mi\n", INT_MAX);
	printf("%s", dest);
	x = oversprintf(dest, "%mu\n", INT_MAX);
	printf("%s", dest);
	x = oversprintf(dest, "%md\n", -312.3125);
	printf("%s", dest);
	x = oversprintf(dest, "%mf\n", -312.3125);
	printf("%s", dest);
	printf("%d\n", x);
}