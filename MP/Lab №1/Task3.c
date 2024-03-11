#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define ull unsigned long long
#define ll long long

enum status_code
{
	ok,
	incorrect_input,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
	wrong_way_error,
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
	"Writing to file error\n",
	"Wrong way to file\n",
	"Unknown error"
};

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

int str_to_double(char* in, double* out)
{
	if (in == NULL || out == NULL)
		return incorrect_input;
	
    errno = 0;
    char* end = NULL;
    *out = strtod(in, &end);
    if(errno == ERANGE)
        return overflow;

    if (errno != 0 && *out == 0)
        return unknown_error;
    
    if(in == end || *end != '\0')
        return incorrect_input;
    
    return ok;

}

int str_to_double_list(char** in, double** out, int first, int last)
{
	if (in == NULL || out == NULL) return incorrect_input;

	*out = calloc(last - first, sizeof(double));
	for(int i = first; i <= last; ++i)
	{
		if (str_to_double(in[i], &((*out)[i-first])) == incorrect_input)
		{
			return incorrect_input;
		}
	}
	return ok;
}

int str_to_ll(char* in, ll* out)
{
	if (in == NULL || out == NULL)
		return incorrect_input;
	
    errno = 0;
    char* end = NULL;
    *out = strtoll(in, &end, 10);
    if(errno == ERANGE)
        return overflow;

    if (errno != 0 && *out == 0)
        return unknown_error;
    
    if(in == end || *end != '\0')
        return incorrect_input;
    
    return ok;
}

int str_to_ll_list(char** in, ll** out, int first, int last)
{
	if (in == NULL || out == NULL) return incorrect_input;

	*out = calloc(last - first, sizeof(ll));
	for(int i = first; i <= last; ++i)
	{
		if (str_to_ll(in[i], &((*out)[i-first])) == incorrect_input)
		{
			return incorrect_input;
		}
	}
	return ok;
}

int square_equation(double eps, double a, double b, double c, double** ans, int* answers_count)
{
	if (ans == NULL || answers_count == NULL || eps < 0) return incorrect_input;

	double discriminant = b * b - 4 * a * c;

	if (a < eps && a > -eps)
	{
		if (b < eps && b > -eps)
		{
			*answers_count = -1;
			return ok;
		}
		*answers_count = 1;
		*ans = (double*) malloc(sizeof(double));
		if (*ans == NULL)
		{
			return malloc_error;
		}
		(*ans[0] = -c / b);
		return ok;
	}
	if (discriminant < -eps)
	{
		*answers_count = 0;
		return ok;
	}
	if (discriminant < eps || sqrt(discriminant) < eps)
	{
		*answers_count = 1;
		*ans = (double*) malloc(sizeof(double));
		if (*ans == NULL)
		{
			return malloc_error;
		}
		(*ans)[0] = -b / (2 * a);
		return ok;
	}
	*answers_count = 2;
	*ans = (double*) malloc(sizeof(double)*2);
		if (*ans == NULL)
		{
			return malloc_error;
		}
	(*ans)[0] = (-b + sqrt(discriminant)) / (2 * a);
	(*ans)[1] = (-b - sqrt(discriminant)) / (2 * a);
	return ok;
}

int swap_2(double* a, double* b)
{
	if (a == NULL || b == NULL) return incorrect_input;
	double v = *a;
	*a = *b;
	*b = v;
	return ok;
}

int swap(double* a, double* b, double* c, int k)
{
	if (a == NULL || b == NULL || c == NULL) return incorrect_input;
	if (k) swap_2(a, b);
	else swap_2(b, c);

	return ok;
}

int q_square_equations(double eps, double a, double b, double c)
{
	if (eps < 0) return incorrect_input;
	int k = 0;
	double* ans;
	int ans_count;
	for (int i = 0; i < 6; ++i)
	{
		printf("%lf * x^2 + (%lf) * x + (%lf) = 0\n", a, b, c);
		square_equation(eps, a, b, c, &ans, &ans_count);
		if (ans_count > 0)
		{
			printf("Answer: \n");
			for (int j = 0; j < ans_count; ++j)
			{
				printf("x%d = %lf\n", j + 1, ans[j]);
			}
			ans_count = 0;
			free(ans);
		}
		else if (ans_count == -1) printf("Root is any number\n");
		else printf("There are no real roots\n");
		printf("\n");

		swap(&a, &b, &c, k);

		k = -k + 1;
	}

	return ok;
}

int ll_overflow(ll* n)
{
	if (*n == LLONG_MAX || *n == LLONG_MIN) return overflow;
	return ok;
}

int m_multiples(ll* a, ll* b, int* ans)
{
	if (*a == 0 || *b == 0 || ans == NULL) return incorrect_input;
	if(ll_overflow(a) || ll_overflow(b)) return overflow;
	*ans = *a % *b == 0;
	return ok;
}

int t_right_triangle(double eps, double a, double b, double c, int* ans)
{
	if (ans == NULL || eps < 0) return incorrect_input;
	if (a <= eps || b <= eps || c <= eps)
	{
		*ans = 0;
		return ok;
	}
	double hyp = max(c, max(a, b));
	if (a * a + b * b + c * c > 2 * hyp * hyp - eps && a * a + b * b + c * c < 2 * hyp * hyp + eps)
	{
		*ans = 1;
	}
	else
	{
		*ans = 0;
	}
	return ok;
}
int main(int argc, char** argv)
{
	if (argc < 4 || strlen(argv[1]) != 2 || argc > 6)
	{
		printf(errors[incorrect_input]);
		printf("Usage: <flag> <num> <num> (<num>)");
		return 1;
	}

	char* flag = argv[1];
	if (!(flag[0] == '-' || flag[0] == '/'))
    {
        printf(errors[incorrect_input]);
        return 1;
    }

	int status = incorrect_input;
	switch (flag[1])
	{
	case 'q':
		if (argc != 6)
		{
			printf(errors[incorrect_input]);
        	return 1;
		}
		
		double* d_nums;
		status = str_to_double_list(argv, &d_nums, 2, 5);
		if (status == ok)
		{
			status = q_square_equations(d_nums[0], d_nums[1], d_nums[2], d_nums[3]);
			free(d_nums);
		}
		break;
	case 'm':
		if (argc != 4)
		{
			printf(errors[incorrect_input]);
        	return 1;
		}

		int ans;
		ll* ll_nums;
		status = str_to_ll_list(argv, &ll_nums, 2, 3);
		if (status == ok)
		{
			status = m_multiples(ll_nums, ll_nums+1, &ans);
			free(ll_nums);
			if (status == ok) printf("%d \n", ans);
		}
		break;
	case 't':
		if (argc != 6)
		{
			printf(errors[incorrect_input]);
        	return 1;
		}
		
		status = str_to_double_list(argv, &d_nums, 2, 5);
		if (status == ok)
		{
			status = t_right_triangle(d_nums[0], d_nums[1], d_nums[2], d_nums[3], &ans);
			free(d_nums);
			printf("%d\n", ans);
		}
		break;
	default:
		printf(errors[incorrect_input]);
		return 1;
	}

	if (status >= unknown_error)
	{
		printf(errors[unknown_error]);
	}
	else if(status != ok)
	{
		printf(errors[status]);
	}

	return status;
}