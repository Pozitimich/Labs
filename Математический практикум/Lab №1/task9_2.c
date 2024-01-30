#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define const_size 8

enum status_code
{
	ok,
	invalid_input,
    invalid_params,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
    file_invalid_data,
	unknown_error
};

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
    "Invalid parameters were passed to the function\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open the file\n",
	"Errors occurred while reading the file\n",
	"Errors occurred while writing to the file\n",
    "The contents of the file contradict the condition of the task\n",
	"Unknown error\n"
};



int fill_rand_nums(int* arr, int len, int a, int b)
{
    if (arr == NULL) return invalid_params;

    int range = b-a+1;
    for (int i = 0; i < len; ++i)
    {
        arr[i] = (int) (rand() % range + a);
    }
    return ok;
}

int swap_2(int*a, int*b)
{
    if (a == NULL || b == NULL) return invalid_params;
    int c = *a;
    *a = *b;
    *b = c;
    return ok;
}

int mediane(int a, int b, int c, int* ans)
{
    if (ans == NULL) return invalid_params;

    int ab = abs(a - b);
    int ac = abs(a - c);
    int bc = abs(b - c);
    if (ab == ac + bc) *ans = 2;
    else if (ac == ab + bc) *ans = 1;
    else *ans = 0;
    return ok;
}

void quick_sort(int* arr, int begin, int end)
{
	if (arr == NULL || begin == end)
	{
		return;
	}
    int mid = (begin + end) / 2;

	int med;
    mediane(arr[begin], arr[end], arr[mid], &med);
    if (med == 0) swap_2(arr + begin, arr + mid);
    else if (med == 1) swap_2(arr + end, arr + mid);

	mid = arr[mid];
	int i = begin;
	int j = end;
	while (i < j)
	{
		while (arr[i] < mid)
		{
			++i;
		}
		while (arr[j] > mid)
		{
			--j;
		}
		if (i < j)
		{
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			++i;
			--j;
		}
	}
	while (arr[j] > mid)
	{
		--j;
	}
	quick_sort(arr, begin, j);
	quick_sort(arr, j+1, end);
}

int search_closest_value(int* arr, int len, int* ans, int val)
{
    if (arr == NULL || ans == NULL) return invalid_params;

    int l = 0, r = len-1, m = (l + r) / 2;
    while (m != l && m != r)
    {
        if (arr[m] < val) l = m;
        else r = m;
        m = (l + r) / 2;
    }
    *ans = (val - arr[l] < arr[r] - val) ? arr[l] : arr[r];
    return ok;
}

int fill_c_array(int* A, int* B, int A_len, int B_len, int* C)
{
    if (A == NULL || B == NULL || C == NULL || A_len < 0 || B_len < 0) return invalid_params;

    int* B1 = (int*) malloc(B_len * sizeof(int));
    if (B1 == NULL) return malloc_error;


    for (int i = 0; i < B_len; ++i) B1[i] = B[i];
    if (B_len != 0) quick_sort(B1, 0, B_len-1);
    int closest_val;
    for(int i = 0; i < A_len; ++i)
    {
        search_closest_value(B1, B_len, &closest_val, A[i]);
        C[i] = A[i] + closest_val;
    }
    free(B1);
    return ok;
}

int array_to_file(int* arr, int len, FILE* file)
{
    if (arr == NULL || file == NULL) return invalid_params;

    for(int i = 0; i < len; ++i)
    {
        if(arr[i] < 0) fputc('-', file);
        int mask = 1, num_copy = abs(arr[i]);
        int c;
        while(mask <= num_copy) mask *= 10;
        while (mask != 1) 
        {
            mask /= 10;
            c = '0' + (num_copy / mask);
            fputc(c, file);
            num_copy %= mask;
        }
        fputc('\n', file);
    }
    return ok;
}

int main()
{
    srand(time(NULL));
    int A_len = (rand()%(10000 - 10 + 1) + 10);
    int* A = (int*) malloc(A_len * sizeof(int));
    if (A == NULL) 
    {
        printf(errors[malloc_error]);
        return malloc_error;
    }
    int B_len = (rand()%(10000 - 10 + 1) + 10);
    int* B = (int*) malloc(B_len * sizeof(int));
    if (B == NULL) 
    {
        printf(errors[malloc_error]);
        return malloc_error;
    }
    fill_rand_nums(A, A_len, -1000, 1000);
    fill_rand_nums(B, B_len, -1000, 1000);

    int* C = (int*) malloc(A_len * sizeof(int));
    int status = fill_c_array(A, B, A_len, B_len, C);
    if(status == ok)
    {
        int* B1 = (int*) malloc(B_len * sizeof(int));
        if (B1 == NULL) 
        {
            printf(errors[malloc_error]);
            return malloc_error;
        }
        for (int i = 0; i < B_len; ++i) B1[i] = B[i];
        if (B_len != 0) quick_sort(B1, 0, B_len-1);

        int* C1 = (int*) malloc(A_len * sizeof(int));
        if (C1 == NULL)
        {
            printf(errors[malloc_error]);
            return malloc_error;
        }
        for(int i = 0; i < A_len; ++i) C1[i] = C[i] - A[i];

        FILE* A_txt = fopen("A.txt", "w");
        array_to_file(A, A_len, A_txt);
        fclose(A_txt);

        FILE* B_txt = fopen("B.txt", "w");
        array_to_file(B, B_len, B_txt);
        fclose(B_txt);

        FILE* B1_txt = fopen("B1.txt", "w");
        array_to_file(B1, B_len, B1_txt);
        fclose(B1_txt);
        
        FILE* C_txt = fopen("C.txt", "w");
        array_to_file(C, A_len, C_txt);
        fclose(C_txt);

        FILE* C1_txt = fopen("C1.txt", "w");
        array_to_file(C1, A_len, C1_txt);
        fclose(C1_txt);
        free(C1);
    }
    printf(errors[status]);

    free(A);
    free(B);
    free(C);
    
    return status;
}