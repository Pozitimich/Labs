#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

typedef enum 
{
	OK,
	INVALID_INPUT,
    INVALID_PARAMS,
    WEIRD_VALUES,
	MY_OVERFLOW,
	MALLOC_ERROR,
	FILE_OPENING_ERROR,
	FILE_READING_ERROR,
	FILE_WRITING_ERROR,
    FILE_INVALID_DATA,
	UNKNOWN_ERROR
}status_code;

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
	unsigned n;
	double* vals;
} vec_n;

typedef struct
{
	unsigned n;
	double** vals;
} matrix_n;

status_code construct_vector(unsigned n, vec_n *vec)
{
	if (vec == NULL) return INVALID_PARAMS;
	
	vec->n = n;
	vec->vals = (double*) malloc(sizeof(double) * n);
	if (vec->vals == NULL) return MALLOC_ERROR;
	
	return OK;
}

status_code fill_vector(vec_n vec, ...)
{
	va_list arg;
	if (vec.vals == NULL) return INVALID_PARAMS;
	
	va_start(arg, vec);
	for (int i = 0; i < vec.n; ++i)
	{
		vec.vals[i] = va_arg(arg, double);
	}
	va_end(arg);
	return OK;
}

status_code destruct_vector(vec_n* vec)
{
	if (vec == NULL) return INVALID_PARAMS;
	
	free(vec->vals);
	vec->vals = NULL;
	vec->n = 0;
	return OK;
}

void print_vector(vec_n vec)
{
	printf("(");
	for (int i = 0; i < vec.n - 1; ++i)
	{
		printf("%.3lf; ", vec.vals[i]);
	}
	printf("%.3lf)", vec.vals[vec.n - 1]);
}

int is_matrix_valid(const matrix_n matrix)
{
	if (matrix.vals == NULL)
	{
		return 0;
	}
	for (int i = 0; i < matrix.n; ++i)
	{
		if (matrix.vals[i] == NULL)
		{
			return 0;
		}
	}
	return 1;
}

status_code construct_matrix(unsigned n, matrix_n *matrix)
{
	if (matrix == NULL) return INVALID_PARAMS;
	
	matrix->n = n;
	matrix->vals = (double**) malloc(sizeof(double*) * n);
	if (matrix->vals == NULL)
	{
		return MALLOC_ERROR;
	}
	for (int i = 0; i < n; ++i)
	{
		matrix->vals[i] = (double*) malloc(sizeof(double) * n);
		if (matrix->vals[i] == NULL)
		{
			for (int j = 0; j < i; ++j)
			{
				free(matrix->vals[j]);
			}
			free(matrix->vals);
			matrix->vals = NULL;
			return MALLOC_ERROR;
		}
	}
	return OK;
}

status_code fill_matrix(matrix_n matrix, ...)
{
	va_list arg;
	if (!is_matrix_valid(matrix))
	{
		return INVALID_PARAMS;
	}
	va_start(arg, matrix);
	for (int i = 0; i < matrix.n; ++i)
	{
		for(int j = 0; j < matrix.n; ++j)
		{
			matrix.vals[i][j] = va_arg(arg, double);
		}
	}
	va_end(arg);
	return OK;
}

status_code destruct_matrix(matrix_n* matrix)
{
	if (!is_matrix_valid(*matrix))
	{
		return INVALID_PARAMS;
	}
	for (int i = 0; i < matrix->n; ++i)
	{
		free(matrix->vals[i]);
	}
	free(matrix->vals);
	matrix->n = 0;
	matrix->vals = NULL;
	return OK;
}

status_code sum_matrix(const matrix_n matrix_l, const matrix_n matrix_r, matrix_n res_matrix)
{
	if (!is_matrix_valid(matrix_l) || !is_matrix_valid(matrix_r) || !is_matrix_valid(res_matrix)
			|| matrix_l.n != matrix_r.n || matrix_r.n != res_matrix.n)
	{
		return INVALID_PARAMS;
	}
	for (int i = 0; i < matrix_l.n; ++i)
	{
		for (int j = 0; j < matrix_l.n; ++j)
		{
			res_matrix.vals[i][j] = matrix_l.vals[i][j] + matrix_r.vals[i][j];
		}
	}
	return OK;
}

status_code multiply_matrix(const matrix_n matrix, double mult, matrix_n res_matrix)
{
	if (!is_matrix_valid(matrix) || !is_matrix_valid(res_matrix) || matrix.n != res_matrix.n)
	{
		return INVALID_PARAMS;
	}
	for (int i = 0; i < matrix.n; ++i)
	{
		for (int j = 0; j < matrix.n; ++j)
		{
			res_matrix.vals[i][j] = mult * matrix.vals[i][j];
		}
	}
	return OK;
}

status_code transpose_matrix(const matrix_n matrix, matrix_n res_matrix)
{
	if (!is_matrix_valid(matrix) || !is_matrix_valid(res_matrix))
	{
		return INVALID_PARAMS;
	}
	for (int i = 0; i < matrix.n; ++i)
	{
		for (int j = 0; j < matrix.n; ++j)
		{
			res_matrix.vals[i][j] = matrix.vals[j][i];
		}
	}
	return OK;
}

status_code factorial(int integer, int* result)
{
	if (result == NULL)
	{
		return INVALID_PARAMS;
	}
	*result = 1;
	for (int i = 2; i <= integer; ++i)
	{
		if (*result > ULLONG_MAX / i)
		{
			return MY_OVERFLOW;
		}
		*result *= i;
	}
	return OK;
}

status_code ll_swap(int* a, int* b)
{
	if (a == NULL || b == NULL) return INVALID_PARAMS;
	
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
	return OK;
}

status_code get_permutations(unsigned inp_len, const int* input_arr, int* ans_len, int*** ans)
{
	status_code status;
	int *cur_perm, fst_pos, sec_pos;
	if (ans_len == NULL || ans == NULL)
	{
		return INVALID_PARAMS;
	}
	if (input_arr == NULL)
	{
		*ans_len = 0;
		return OK;
	}
	
	status = factorial(inp_len, ans_len);
	if (status != OK)
	{
		return status;
	}
	
	*ans = (int**) malloc(sizeof(int*) * (*ans_len));
	if (*ans == NULL)
	{
		return MALLOC_ERROR;
	}
	
	for (int i = 0; i < *ans_len; ++i)
	{
		(*ans)[i] = (int*) malloc(sizeof(int) * inp_len);
		
		if ((*ans)[i] == NULL)
		{
			for (int j = 0; j < i; ++j)
			{
				free((*ans)[i]);
			}
			free(*ans);
			*ans = NULL;
			return MALLOC_ERROR;
		}
	}
	
	for (int j = 0; j < inp_len; ++j)
	{
		(*ans)[0][j] = input_arr[j];
	}
	
	for (int i = 1; i < *ans_len; ++i)
	{
		for (int j = 0; j < inp_len; ++j)
		{
			(*ans)[i][j] = (*ans)[i - 1][j];
		}

		cur_perm = (*ans)[i];
		fst_pos = inp_len - 2;
		sec_pos = inp_len - 1;
		
		while (fst_pos != -1 && cur_perm[fst_pos] >= cur_perm[fst_pos + 1])
		{
			--fst_pos;
		}
		
		if (fst_pos == -1)
		{
			for (int j = 0; 2 * j < inp_len; ++j)
			{
				ll_swap(cur_perm + j, cur_perm + (inp_len - j - 1));
			}
		}
		else
		{
			while (cur_perm[sec_pos] <= cur_perm[fst_pos]) 
			{
				--sec_pos;
			}
			
			ll_swap(cur_perm + fst_pos, cur_perm + sec_pos);
			
			for (int j = 1; 2 * j < inp_len - fst_pos; ++j)
			{
				ll_swap(cur_perm + (fst_pos + j), cur_perm + (inp_len - j));
			}	
		}
	}
	return OK;
}

status_code calc_matrix_det(unsigned n, const matrix_n matrix, double* det)
{
	int* indexes;
	int perm_cnt;
	int** perms;
	status_code status;
	double monomial;
	if (!is_matrix_valid(matrix) || det == NULL || n > matrix.n)
	{
		return INVALID_PARAMS;
	}
	
	indexes = (int*) malloc(sizeof(int) * n);
	if (indexes == NULL)
	{
		return MALLOC_ERROR;
	}
	for (int i = 0; i < n; ++i)
	{
		indexes[i] = i;
	}
	
	status = get_permutations(n, indexes, &perm_cnt, &perms);
	free(indexes);
	if (status != OK)
	{
		return status;
	}
	
	*det = 0;
	for (int i = 0; i < perm_cnt; ++i)
	{
		indexes = perms[i];
		monomial = 1;
		for (int j = 0; j < n; ++j)
		{
			monomial *= matrix.vals[j][indexes[j]];
		}
		if (((i + 1) / 2) & 1)
		{
			monomial *= -1;
		}
		*det += monomial;
	}
	
	for (int i = 0; i < perm_cnt; ++i)
	{
		free(perms[i]);
	}
	free(perms);
	return OK;
}

status_code is_matrix_positive_definite(const matrix_n matrix, double eps, int* ans)
{
	double det;
	status_code status;
	if (!is_matrix_valid(matrix) || ans == NULL)
	{
		return INVALID_PARAMS;
	}
	if (eps <= 0)
	{
		return INVALID_INPUT;
	}
	for (int i = 1; i <= matrix.n; ++i)
	{
		status = calc_matrix_det(i, matrix, &det);
		if (status != OK)
		{
			return status;
		}
		if (det < eps)
		{
			*ans = 0;
			return OK;
		}
	}
	*ans = 1;
	return OK;
}

status_code calc_norm_inf(const vec_n vec, double* norm)
{
	double mx = 0;
	if (norm == NULL || vec.vals == NULL)
	{
		return INVALID_PARAMS;
	}
	for (int i = 0; i < vec.n; ++i)
	{
		mx = fmax(mx, fabs(vec.vals[i]));
	}
	*norm = mx;
	return OK;
}

status_code calc_norm_p(const vec_n vec, int p, double* norm)
{
	double sum = 0;
	if (norm == NULL || vec.vals == NULL)
	{
		return INVALID_PARAMS;
	}
	if (p < 1)
	{
		return INVALID_PARAMS;
	}
	
	for (int i = 0; i < vec.n; ++i)
	{
		sum += pow(fabs(vec.vals[i]), p);
	}
	*norm = pow(sum, 1.0 / p);
	return OK;
}

status_code calc_norm_matrix(const vec_n vec, const matrix_n matrix, double eps, double* norm)
{
	vec_n vec_l;
	status_code status;
	int is_posit_def;
	if (vec.vals == NULL || !is_matrix_valid(matrix) || norm == NULL || vec.n != matrix.n)
	{
		return INVALID_PARAMS;
	}
	if (eps <= 0)
	{
		return INVALID_INPUT;
	}
	status = is_matrix_positive_definite(matrix, eps, &is_posit_def);
	if (status != OK)
	{
		return status;
	}
	if (!is_posit_def)
	{
		return INVALID_PARAMS;
	}
	
	vec_l.n = vec.n;
	vec_l.vals = (double*) malloc(sizeof(double) * vec.n);
	if (vec_l.vals == NULL)
	{
		return MALLOC_ERROR;
	}
	
	for (int i = 0; i < vec.n; ++i)
	{
		vec_l.vals[i] = 0;
		for (int j = 0; j < vec.n; ++j)
		{
			vec_l.vals[i] += matrix.vals[i][j] * vec.vals[j];
		}
	}
	
	*norm = 0;
	for (int i = 0; i < vec.n; ++i)
	{
		*norm += vec_l.vals[i] * vec.vals[i];
 	}
	*norm = sqrt(*norm);
	
	free(vec_l.vals);
	return OK;
}

typedef struct
{
	int len;
	vec_n* vectors;
}Calc_inf_input;

typedef struct
{
	int len;
	int p;
	vec_n* vectors;
}Calc_p_input;

typedef struct
{
	int len;
	matrix_n matrix;
	double eps;
	vec_n* vectors;
}Calc_matrix_input;

status_code calc_norm_inf_args(double** norms, vec_n** vecs, int* ans_len, va_list* arg)
{
	if(norms == NULL || vecs == NULL || ans_len == NULL || arg == NULL) return INVALID_PARAMS;
	
	Calc_inf_input inp;
	int i;
	status_code status;
	inp = va_arg(*arg, Calc_inf_input);
	*norms = (double*) malloc(sizeof(double)*inp.len); 
	if (inp.vectors == NULL) return MALLOC_ERROR;

	*vecs = (vec_n*) malloc(sizeof(vec_n)*inp.len); 
	if (inp.vectors == NULL) 
	{
		free(*norms);
		return MALLOC_ERROR;
	}
	for (i = 0; i < inp.len; ++i)
	{
		(*vecs)[i] = (inp.vectors)[i];
		status = calc_norm_inf((*vecs)[i], *norms+i);
		if (status)
		{
			free(*vecs);
			free(*norms);
			return status;
		}
	}
	*ans_len = inp.len;
	return OK;
}
status_code calc_norm_p_args(double** norms, vec_n** vecs, int* ans_len, va_list* arg)
{
	if(norms == NULL || vecs == NULL || ans_len == NULL || arg == NULL) return INVALID_PARAMS;
	
	Calc_p_input inp;
	int i;
	status_code status;
	inp = va_arg(*arg, Calc_p_input);
	*norms = (double*) malloc(sizeof(double)*inp.len); 
	if (inp.vectors == NULL) return MALLOC_ERROR;

	*vecs = (vec_n*) malloc(sizeof(vec_n)*inp.len); 
	if (inp.vectors == NULL) 
	{
		free(*norms);
		return MALLOC_ERROR;
	}
	for (i = 0; i < inp.len; ++i)
	{
		(*vecs)[i] = (inp.vectors)[i];
		status = calc_norm_p((*vecs)[i], inp.p, *norms+i);
		if (status)
		{
			free(*vecs);
			free(*norms);
			return status;
		}
	}
	*ans_len = inp.len;
	return OK;
}
status_code calc_norm_matrix_args(double** norms, vec_n** vecs, int* ans_len, va_list* arg)
{
	if(norms == NULL || vecs == NULL || ans_len == NULL || arg == NULL) return INVALID_PARAMS;
	
	Calc_matrix_input inp;
	int i;
	status_code status;
	inp = va_arg(*arg, 	Calc_matrix_input);

	if (inp.len == 0) 
	{
		*ans_len = 0;
		return OK;
	}
	*norms = (double*) malloc(sizeof(double)*inp.len); 
	if (inp.vectors == NULL) return MALLOC_ERROR;

	*vecs = (vec_n*) malloc(sizeof(vec_n)*inp.len); 
	if (inp.vectors == NULL) 
	{
		free(*norms);
		return MALLOC_ERROR;
	}
	for (i = 0; i < inp.len; ++i)
	{
		(*vecs)[i] = (inp.vectors)[i];
		status = calc_norm_matrix((*vecs)[i], inp.matrix, inp.eps, *norms+i);
		if (status)
		{
			free(*vecs);
			free(*norms);
			return status;
		}
	}
	*ans_len = inp.len;
	return OK;
}

status_code solve
(
	unsigned n, double eps, int func_cnt, int** res_cnts, vec_n*** res_vecs, 
	status_code (*funcs[])(double**, vec_n**, int*, va_list*), ...
)
{
	int i, j, k, cnt;
	double* norms;
	vec_n* vectors;
	status_code status;
	va_list arg;

	double max;

	*res_vecs = (vec_n**) malloc(sizeof(vec_n*) * func_cnt);
	if (*res_vecs == NULL) return MALLOC_ERROR;
	*res_cnts = (int*) malloc(sizeof(int) * func_cnt);
	if (*res_cnts == NULL) 
	{
		free(*res_vecs);
		return MALLOC_ERROR;
	}
	va_start(arg, 100);
	for(i = 0; i < func_cnt; ++i)
	{
		status = funcs[i](&norms, &vectors, &cnt, &arg);
		if (status) 
		{
			va_end(arg);
			free(*res_cnts);
			free(*res_vecs);
			return status;
		}
		if (!cnt) continue;
		(*res_cnts)[i] = 1;
		max = norms[0];
		for (j = 1; j < cnt; ++j)
		{
			if (norms[j] > max - eps && norms[j] < max + eps)
			{
				++(*res_cnts)[i];
			}
			else if (norms[j] > max)
			{
				max = norms[j];
				(*res_cnts)[i] = 1;
			}
		}
		(*res_vecs)[i] = (vec_n*) malloc(sizeof(vec_n) * (*res_cnts)[i]);
		if (res_vecs == NULL)
		{
			va_end(arg);
			free(*res_cnts);
			free(*res_vecs);
			free(norms);
			return MALLOC_ERROR;
		}
		for (k = 0, j = 0; j < cnt; ++j)
		{
			if (norms[j] > max - eps && norms[j] < max + eps)
			{
				(*res_vecs)[i][k++] = vectors[j];
				if (k == (*res_cnts)[i]) break;
			}
		}
	}
	free(norms);
	va_end(arg);
	return OK;
}

int main(int argc, char** argv)
{
	unsigned n = 2;
	matrix_n matrix;
	vec_n vecs[3];
	status_code status;
	double eps = 1e-15;
	int* cnts;
	vec_n** res_vecs;
	status_code (*funcs[3])(double**, vec_n**, int*, va_list*)
		= {calc_norm_inf_args, calc_norm_p_args, calc_norm_matrix_args};
	int p = 3, i, j;

	construct_matrix(n, &matrix);
	fill_matrix(matrix, 5.0, 10.0, -5.0, 2.0);
	
	construct_vector(n, vecs);
	construct_vector(n, vecs+1);
	construct_vector(n, vecs+2);
	fill_vector(vecs[0], 0.0, 5.0);
	fill_vector(vecs[1], 1.0, 1.0);
	fill_vector(vecs[2], 4.001, 4.001);
	status = solve(n, eps, 3, &cnts, &res_vecs, funcs, (Calc_inf_input) {3, vecs}, 
		(Calc_p_input) {3, p, vecs}, (Calc_matrix_input) {3, matrix, eps, vecs});
	if (status)
	{
		printf(errors[status]);
		return status;
	}
	
	for(i = 0; i < 3; ++i)
	{
		printf("%d ", cnts[i]);
		destruct_vector(vecs+i);
	}
	printf("\n");
	for (i = 0; i < 3; ++i)
	{
		printf("vecds with max norm%d: ", i+1);
		for (int j = 0; j < cnts[i]; ++j)
		{
			print_vector(res_vecs[i][j]);
			printf(" ");
			destruct_vector(res_vecs[i]+j);
		};
		printf("\n");
		free(res_vecs[i]);
	}
	free(res_vecs);
	free(cnts);
	destruct_matrix(&matrix);
	return 0;
}