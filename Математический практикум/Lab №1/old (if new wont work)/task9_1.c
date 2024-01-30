#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "my_library.h"
#define const_size 8

int fill_rand_nums(int* arr, int len, int a, int b)
{
    if (arr == NULL) return invalid_params;

    int c = a;
    if (a > b)
    {
        a = b;
        b = c;
    }
    
    int range = b-a+1;
    for (int i = 0; i < len; ++i)
    {
        arr[i] = (int) (rand() % range + a);
    }
    return ok;
}

int swap_min_max(int* arr, int len)
{
    if (arr == NULL || len == 0) return invalid_params;

    int min_el = arr[0], max_el = arr[0];
    int min_index = 0, max_index = 0;
    for(int i = 0; i < len; ++i)
    {
        if (min_el > arr[i])
        {
            min_el = arr[i];
            min_index = i;
        }
        if (max_el < arr[i])
        {
            max_el = arr[i];
            max_index = i;
        }
    }
    arr[min_index] = max_el;
    arr[max_index] = min_el;
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

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage: <A> <B>, A < B, integers\n");
        return invalid_input;
    }

    int a, b;
    int status = str_to_int(argv[1], &a);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    status = str_to_int(argv[2], &b);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    int arr_1[const_size], arr_1_copy[const_size];
    srand(time(NULL));
    fill_rand_nums(arr_1, const_size, a, b);
    for (int i = 0; i < const_size; ++i) arr_1_copy[i] = arr_1[i];

    status = swap_min_max(arr_1, const_size);
    if (status != ok)
    {
        printf(errors[status]);
        return status;
    }
    printf("Before processing: ");
    for (int i = 0; i < const_size; ++i) printf("%d ", arr_1_copy[i]);
    printf("\n");
    printf("After processing: ");
    for (int i = 0; i < const_size; ++i) printf("%d ", arr_1[i]);
    printf("\n");

    return 0;
}