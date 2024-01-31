#include <stdio.h>
#include <stdarg.h>

typedef enum 
{
	ok,
	invalid_input,
    invalid_params,
    weird_values,
	overflow,
	malloc_error,
	file_opening_error,
	file_reading_error,
	file_writing_error,
    file_invalid_data,
	unknown_error
} status_code;

static const char* errors[] =
{
	"Success\n",
	"Invalid input\n",
    "Invalid parameters were passed to the function\n",
    "The problem is unsolvable for the given arguments\n",
	"Overflow error\n",
	"Memory allocation error\n",
	"Cannot open the file\n",
	"Errors occurred while reading the file\n",
	"Errors occurred while writing to the file\n",
    "The contents of the file contradict the condition of the task\n",
	"Unknown error\n"
};

typedef struct 
{
    int x,y;
} vec2;

vec2 vec_diff(vec2 a, vec2 b)
{
    return (vec2) {a.x - b.x, a.y - b.y};
}

int rotat_dir(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

status_code is_convex(int* ans, int n, ...)
{
    if (ans == NULL || n < 3) return invalid_params;

    *ans = 1;
    va_list arg;
    va_start(arg, n);

    vec2 point1 = va_arg(arg, vec2);
    vec2 point2 = va_arg(arg, vec2);
    vec2 point3, vector12, vector23;
    int cur_rotat_dir = 0;
    for(int i = 0; i < n; ++i)
    {
        if (i == n-2) va_start(arg,n);
        point3 = va_arg(arg, vec2);
        vector12 = vec_diff(point2, point1);
        vector23 = vec_diff(point3, point2);
        if (cur_rotat_dir == 0)
        {
            double this_rotat_dir = rotat_dir(vector12, vector23);
            if (this_rotat_dir > 0) cur_rotat_dir = 1;
            else if (this_rotat_dir < 0) cur_rotat_dir = -1; 
        }
        else if (cur_rotat_dir * rotat_dir(vector12, vector23) < 0)
        {
            *ans = 0;
            return ok;
        }
        point1 = point2;
        point2 = point3;
    }
    if (cur_rotat_dir == 0) *ans = 0;
    return ok;
}

int main(int argc, char** argv)
{
    int ans, status;
    status = is_convex(&ans, 4, (vec2) {0,0}, (vec2) {0,3}, (vec2) {3,0}, (vec2) {4,1});
    printf(errors[status]);
    printf("%d\n", ans);
    return status;
}
