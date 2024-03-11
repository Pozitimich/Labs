#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


typedef unsigned long long ull;
typedef long long ll;

typedef enum
{
    OK,
    INVALID_INPUT,
    INVALID_PARAMS,
    WEIRD_VALUES,
    OVERFLOW_ERROR,
    ALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_INVALID_DATA,
    ACCESS_ERROR,
    UNKNOWN_ERROR,
    RESERVED
}status_code;

void print_error(status_code status)
{
    switch (status)
    {
    case OK:
        printf("Success\n");
        return;
    case INVALID_INPUT:
        printf("Invalid input\n");
        return;
    case INVALID_PARAMS:
        printf("Invalid parameters were passed to the function\n");
        return;
    case WEIRD_VALUES:
        printf("The problem is unsolvable for the given arguments\n");
        return;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return;
    case FILE_OPENING_ERROR:
        printf("Cannot open the file\n");
        return;
    case FILE_INVALID_DATA:
        printf("The contents of the file contradict the condition of the task\n");
        return;
    case ALLOC_ERROR:
        printf("Memory allocation error\n");
        return;
    case ACCESS_ERROR:
        printf("Access error\n");
        return;
    case UNKNOWN_ERROR:
        printf("Unknown error\n");
        return;
    case RESERVED:
        printf("Reserved code\n");
        return;
    default:
        printf("Unexpected code\n");
        return;
    }
}

typedef struct
{
    char* name;
    ll val;
} Memory_cell;

status_code search_mcell(Memory_cell** cells, const ull cnt, char* name, ll* ans)
{
    ull left = 0, right = cnt, mid;
    int cmp;
    if (cells == NULL || name == NULL || ans == NULL) return INVALID_PARAMS;
    if (cnt == 0) return WEIRD_VALUES;
    while (left < right)
    {
        mid = (left + right) / 2;
        cmp = strcmp(name, cells[mid]->name);
        if (cmp > 0)
        {
            left = mid + 1;
        }
        else if (cmp < 0)
        {
            right = mid;
        }
        else
        {
            *ans = mid;
            return OK;
        }
    }

    return WEIRD_VALUES;
}

status_code get_value(Memory_cell** cells, const ull cnt, char* arg, ll* ans)
{
    ll tmp;
    char c;
    int i;
    status_code status;
    if (cells == NULL || arg == NULL || ans == NULL) return INVALID_PARAMS;

    *ans = 0;
    c = arg[0];
    if (c == '-' || c >= '0' && c <= '9')
    {
        for (i = (c == '-'); arg[i]; ++i)
        {
            c = arg[i];
            if (c < '0' || c > '9') return INVALID_INPUT;
            if (LLONG_MAX / 10 < *ans) return OVERFLOW_ERROR;
            *ans *= 10;
            *ans += arg[i] - '0';
        }
        if (arg[0] == '-') *ans = -(*ans);
    }
    else
    {
        for (i = 1; arg[i]; ++i)
        {
            if (!isalpha(c)) return INVALID_INPUT;
            c = arg[i];
        }
        status = search_mcell(cells, cnt, arg, ans);
        if (status) return status;
        *ans = cells[*ans]->val;
    }
    return OK;
}

int is_operation(const char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
        return 1;
    default:
        return 0;
    }
}

typedef struct
{
    char* str;
    ull size;
    ull cnt;
} String;

status_code fread_word(FILE* file, String* word, const int (*separator)(char), char* c)
{
    char* tmp;
    if (file == NULL || c == NULL || separator == NULL) return INVALID_PARAMS;
    word->cnt = 0;
    while (separator(*c) && !feof(file))
    {
        if (word->cnt == word->size - 1)
        {
            word->size *= 2;
            tmp = realloc(word->str, sizeof(char) * word->size);
            if (tmp == NULL)
            {
                free(word->str);
                return ALLOC_ERROR;
            }
            word->str = tmp;
        }
        (word->str)[word->cnt] = *c;
        ++(word->cnt);
        *c = fgetc(file);
    }
    (word->str)[word->cnt] = '\0';
    return OK;
}

int is_not_sep(const char c)
{
    return (c != ' ' && c != '\t' && c != '\n');
}

status_code skip_sep(FILE* file, char* c)
{
    if (file == NULL || c == NULL) return INVALID_PARAMS;
    while (!is_not_sep(*c))
    {
        *c = fgetc(file);
    }
    return OK;
}

int is_alpha(char c)
{
    return isalpha(c);
}

int is_valid(char c)
{
    return c == '-' || isdigit(c) || isalpha(c);
}

status_code perform_operation(const ll arg1, const ll arg2, const char op, ll* ans)
{
    if (ans == NULL) return INVALID_PARAMS;

    switch (op)
    {
    case '+':
        if ((arg1 > 0 && arg2 > 0 && LLONG_MAX - arg2 < arg1)
            || (arg1 < 0 && arg2 < 0 && LLONG_MIN - arg2 > arg1)) return OVERFLOW_ERROR;
        *ans = arg1 + arg2;
        return OK;
    case '-':
        if ((arg1 > 0 && arg2 < 0 && LLONG_MAX + arg2 < arg1)
            || (arg1 < 0 && arg2 > 0 && LLONG_MIN + arg2 > arg1)) return OVERFLOW_ERROR;
        *ans = arg1 - arg2;
        return OK;
    case '*':
        if (arg1 < -1 || arg1 > 1 && arg2 < -1 || arg2 > 1)
        {
            if (arg1 > 0 && arg2 > 0 && LLONG_MAX / arg1 < arg2) return OVERFLOW_ERROR;
            if (arg1 < 0 && arg2 < 0 && LLONG_MAX / -arg1 < -arg2) return OVERFLOW_ERROR;
            if (arg1 < 0 && arg2 > 0 && LLONG_MIN / arg2 > arg1) return OVERFLOW_ERROR;
            if (arg1 > 0 && arg2 < 0 && LLONG_MIN / arg1 > arg2) return OVERFLOW_ERROR;
        }
        *ans = arg1 * arg2;
        return OK;
    case '/':
        *ans = arg1 / arg2;
        return OK;
    case '%':
        *ans = arg1 % arg2;
        return OK;
    default:
        return INVALID_PARAMS;
    }
}

int mem_cmp(const void* l, const void* r)
{
    Memory_cell* lm = *(Memory_cell**)(l);
    Memory_cell* rm = *(Memory_cell**)(r);

    return strcmp(lm->name, rm->name);
}

status_code fread_expr(FILE* file, Memory_cell*** cells_ptr, ull* cnt, ull* size)
{
    status_code status;
    char c, * tmp, op;
    String word;
    ll* arg1, args[2];
    ull index;
    Memory_cell** cells = *cells_ptr;

    if (file == NULL || cells_ptr == NULL) INVALID_PARAMS;

    word.cnt = 0;
    word.size = 2;
    word.str = (char*)malloc(sizeof(char) * word.size);
    if (word.str == NULL) return ALLOC_ERROR;
    c = fgetc(file);
    skip_sep(file, &c);
    if (feof(file)) return OK;
    status = fread_word(file, &word, is_alpha, &c);
    if (status)
    {
        free(word.str);
        return status;
    }
    if (*(word.str) == '\0')
    {
        free(word.str);
        return FILE_INVALID_DATA;
    }

    skip_sep(file, &c);
    if (feof(file))
    {
        free(word.str);
        return FILE_INVALID_DATA;
    }
    if (!strcmp(word.str, "print"))
    {
        if (c == ';')
        {
            free(word.str);
            for (index = 0; index < *cnt; ++index)
            {
                printf("%s: %lld\n", cells[index]->name, cells[index]->val);
            }
            return OK;
        }
        status = fread_word(file, &word, is_alpha, &c);
        if (status)
        {
            free(word.str);
            return status;
        }
        skip_sep(file, &c);
        if (c != ';')
        {
            free(word.str);
            return FILE_INVALID_DATA;
        }
        status = search_mcell(cells, *cnt, word.str, &index);
        free(word.str);
        if (status) return status;

        printf("%lld\n", cells[index]->val);
        return OK;
    }
    status = search_mcell(cells, *cnt, word.str, &index);
    if (status == WEIRD_VALUES)
    {
        tmp = word.str;
        word.str = (char*)realloc(word.str, sizeof(char) * (word.cnt + 1));
        if (tmp == NULL)
        {
            free(word.str);
            return ALLOC_ERROR;
        }

        *cells_ptr = cells;
        if (*cnt == *size)
        {
            *size *= 2;
            cells = (Memory_cell**)realloc(cells, sizeof(Memory_cell*) * (*size));
            if (cells == NULL)
            {
                free(word.str);
                return ALLOC_ERROR;
            }
        }
        cells[*cnt] = (Memory_cell*)malloc(sizeof(Memory_cell));
        if (cells[*cnt] == NULL)
        {
            free(word.str);
            return ALLOC_ERROR;
        }
        cells[*cnt]->name = word.str;

        arg1 = &(cells[*cnt]->val);
        ++(*cnt);
        qsort(cells, *cnt, sizeof(Memory_cell*), mem_cmp);
        *cells_ptr = cells;

        word.cnt = 0;
        word.size = 2;
        word.str = (char*)malloc(sizeof(char) * 2);
        if (word.str == NULL) return ALLOC_ERROR;
    }
    else if (status)
    {
        free(word.str);
        return status;
    }
    else
    {
        arg1 = &(cells[index]->val);
    }

    if (c == ';') return OK;
    if (c != '=')
    {
        return FILE_INVALID_DATA;
    }
    c = fgetc(file);
    skip_sep(file, &c);
    if (feof(file))
    {
        free(word.str);
        return FILE_INVALID_DATA;
    }

    status = fread_word(file, &word, is_valid, &c);
    if (status)
    {
        free(word.str);
        return status;
    }
    skip_sep(file, &c);
    if (c != ';' && !is_operation(c))
    {
        free(word.str);
        return FILE_INVALID_DATA;
    }
    status = get_value(cells, *cnt, word.str, args);
    if (status)
    {
        free(word.str);
        return status;
    }
    if (c == ';')
    {
        *arg1 = args[0];
        free(word.str);
        return OK;
    }
    op = c;
    c = fgetc(file);
    skip_sep(file, &c);
    status = fread_word(file, &word, is_valid, &c);
    if (status)
    {
        free(word.str);
        return status;
    }
    skip_sep(file, &c);
    if (c != ';')
    {
        free(word.str);
        return FILE_INVALID_DATA;
    }
    status = get_value(cells, *cnt, word.str, args + 1);
    free(word.str);
    if (status) return status;
    *cells_ptr = cells;
    return perform_operation(args[0], args[1], op, arg1);
}

status_code finterpet(FILE* file, Memory_cell*** cells, ull* cnt)
{
    status_code status;
    ull size = 2, i;
    Memory_cell** tmp;
    if (file == NULL || cnt == NULL) return INVALID_PARAMS;

    *cells = (Memory_cell**) malloc(sizeof(Memory_cell*)*size);
    if (*cells == NULL) return ALLOC_ERROR;
    while (!feof(file))
    {
        status = fread_expr(file, cells, cnt, &size);
        if (status)
        {
            for (i = 0; i < *cnt; ++i)
            {
                free((*cells)[i]->name);
                free((*cells)[i]);
            }
            free(*cells);
            return status;
        }
    }
    tmp = (Memory_cell**) realloc(*cells, sizeof(Memory_cell*) * (*cnt));
    if (tmp == NULL)
    {
        for (i = 0; i < *cnt; ++i)
        {
            free((*cells)[i]->name);
            free((*cells)[i]);
        }
        free(*cells);
        return UNKNOWN_ERROR;
    }
    return OK;
}

int main()
{
    int argc = 2;
    char* argv = "C:/Users/spiri/Desktop/VS_Labs/Lab_4/7_inp.txt";
    FILE* file;
    status_code status;
    Memory_cell** cells;
    ull cells_cnt = 0, i;
    if (argc != 2)
    {
        printf("Usage: <path>\n");
        return INVALID_INPUT;
    }

    fopen_s(&file, argv, "r");
    if (file == NULL)
    {
        return FILE_OPENING_ERROR;
    }

    status = finterpet(file, &cells, &cells_cnt);
    fclose(file);
    print_error(status);
    if (status)
    {
        return status;
    }
    for (i = 0; i < cells_cnt; ++i)
    {
        free(cells[i]->name);
        free(cells[i]);
    }
    free(cells);
    return 0;
}