#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>

typedef enum 
{
	OK,
	INVALID_INPUT,
    INVALID_PARAMS,
    WEIRD_VALUES,
	OVERFLOW,
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
    unsigned long long id;
    char* name;
    char* surname;
    double salary;
}Employee;

int ascend_comparator(const void* left, const void* right)
{
    Employee* l = *((Employee**) left);
    Employee* r = *((Employee**) right);
    if (l->salary != r->salary) return (l->salary > r->salary) ? -1 : 1;

    int cmp_code = strcmp(l->name, r->name);
    if (cmp_code) return cmp_code;

    cmp_code = strcmp(l->surname, r->surname);
    if (cmp_code) return cmp_code;

    return (l->id < r->id) ? -1 : 1;
}

int descend_comparator(const void* left, const void* right)
{
    Employee* l = *((Employee**) left);
    Employee* r = *((Employee**) right);
    if (l->salary != r->salary) return (l->salary > r->salary) ? 1 : -1;

    int cmp_code = -strcmp(l->name, r->name);
    if (cmp_code) return cmp_code;

    cmp_code = -strcmp(l->surname, r->surname);
    if (cmp_code) return cmp_code;

    return (l->id < r->id) ? 1 : -1;
}

status_code skip_spaces(FILE* in, char* c)
{
    if (in == NULL || c == NULL) return INVALID_PARAMS;

    while (*c == ' ' || *c == '\t') *c = fgetc(in);
    return OK;
}

status_code read_employee(FILE* in, Employee** emp)
{
    if (in == NULL || emp == NULL) return INVALID_PARAMS;

    char c = fgetc(in);
    skip_spaces(in, &c);
    while (c == '\n') c = fgetc(in);
    if (feof(in)) 
    {
        *emp = NULL;
        return OK;
    }

    *emp = (Employee*) malloc(sizeof(Employee));
    if (*emp == NULL) return MALLOC_ERROR;
    
    status_code status = OK;
    (*emp)->id = 0;
    while(c != ' ' && c != '\t')
    {
        if (!isdigit(c)) status = FILE_INVALID_DATA;
        else if (ULLONG_MAX / 10 - (c - '0') < (*emp)->id) status = OVERFLOW;

        if (status)
        {
            free(*emp);
            return status;
        }

        (*emp)->id *= 10;
        (*emp)->id += (c - '0');
        c = fgetc(in);
    }

    skip_spaces(in, &c);
    
    int str_length = 2;
    (*emp)->name = (char*) malloc(str_length * sizeof(char));
    if ((*emp)->name == NULL) return MALLOC_ERROR;
    for(int i = 0; ; ++i)
    {
        if (c == ' ' || c == '\t')
        {
            str_length = i+1;
            break;
        }
        if (!isalpha(c))
        {
            status = FILE_INVALID_DATA;
        }
        else
        {
            if (i == str_length)
            {
                if (INT_MAX / 2 < str_length) return OVERFLOW;
                str_length *= 2;
                (*emp)->name = (char*) realloc((*emp)->name, str_length * sizeof(char));
                if ((*emp)->name == NULL) return MALLOC_ERROR;
            }
            ((*emp)->name)[i] = c;
        }
        c = fgetc(in);
    }
    (*emp)->name = (char*) realloc((*emp)->name, str_length * sizeof(char));
    ((*emp)->name)[str_length-1] = '\0';

    skip_spaces(in, &c);

    str_length = 2;
    (*emp)->surname = (char*) malloc(str_length * sizeof(char));
    if ((*emp)->surname == NULL) return MALLOC_ERROR;
    for(int i = 0; ; ++i)
    {
        if (c == ' ' || c == '\t')
        {
            str_length = i+1;
            break;
        }
        if (!isalpha(c)) status = FILE_INVALID_DATA;
        else
        {
            if (i == str_length)
            {
                if (INT_MAX / 2 < str_length) return OVERFLOW;
                str_length *= 2;
                (*emp)->surname = (char*) realloc((*emp)->surname, str_length * sizeof(char));
                if ((*emp)->surname == NULL) return MALLOC_ERROR;
            }
            ((*emp)->surname)[i] = c;
        }
        c = fgetc(in);
    }
    (*emp)->surname = (char*) realloc((*emp)->surname, str_length * sizeof(char));
    ((*emp)->surname)[str_length-1] = '\0';

    skip_spaces(in, &c);

    (*emp)->salary = 0;
    while(!feof(in) && c != ' ' && c != '\t' && c != '\n' && c != '.')
    {
        if (!isdigit(c)) 
        {
            free((*emp)->name);
            free((*emp)->surname);
            free(*emp);
            return FILE_INVALID_DATA;
        }
        (*emp)->salary *= 10;
        (*emp)->salary += (c - '0');
        c = fgetc(in);
    }
    if (c == '.')
    {
        unsigned long long denom;
        while(!feof(in) && c != ' ' && c != '\t' && c != '\n')
        {
            if (!isdigit(c)) status = FILE_INVALID_DATA;
            else if ((DBL_MAX / 10 - (c -'0') < (*emp)->salary) || (ULLONG_MAX / 10 < denom)) status = OVERFLOW;

            if (status)
            {
                free((*emp)->name);
                free((*emp)->surname);
                free(*emp);
                return status;
            }
            (*emp)->salary *= 10;
            (*emp)->salary += (c - '0');
            denom *= 10;
            c = fgetc(in);
        }
        (*emp)->salary /= denom;
    }
    skip_spaces(in, &c);
    if (!feof(in) && c != '\n') 
    {
        free((*emp)->name);
        free((*emp)->surname);
        free(*emp);
        return FILE_INVALID_DATA;
    }
    return OK;
}

status_code read_employees(FILE* in, Employee*** emps, int* cnt)
{
    if (in == NULL || emps == NULL) return INVALID_PARAMS;
    *cnt = 0;
    if (feof(in))
    {
        return OK;
    }
    Employee* emp, **tmp;
    int status;
    unsigned long long size = 2;

    *emps = (Employee**) malloc(sizeof(Employee*) * size);
    do
    {
        status = read_employee(in, &emp);
        if (status) 
        {
            free(*emps);
            return status;
        }
        if (*cnt == size)
        {
            size *= 2;
            tmp = (Employee**) realloc(*emps, sizeof(Employee*) * size);
            if (tmp == NULL)
            {
                free(*emps);
                return MALLOC_ERROR;
            }
            *emps = tmp;
        }
        (*emps)[*cnt] = emp;
        ++(*cnt);
        
    }while (!feof(in) && emp != NULL);
    tmp = (Employee**) realloc(*emps, sizeof(Employee*) * (*cnt));
    if (tmp == NULL)
    {
        free(*emps);
        return MALLOC_ERROR;
    }
    *emps = tmp;
    return OK;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        printf("Usage: <flag> <input file> <output file>");
        return INVALID_INPUT;
    }

    if ((argv[1][0] != '-' && argv[1][0] != '/') 
        || (argv[1][1] != 'a' && argv[1][1] != 'd') || argv[1][2] != '\0')
    {
        printf(errors[INVALID_INPUT]);
        return INVALID_INPUT;
    }

    FILE* in = fopen(argv[2], "r");
    if (in == NULL) 
    {
        printf(errors[INVALID_INPUT]);
        return INVALID_INPUT;
    }
    FILE* out = fopen(argv[3], "w");
    if (out == NULL)
    {
        fclose(in);
        printf(errors[INVALID_INPUT]);
        return INVALID_INPUT;
    }

    Employee** emps;
    int cnt;
    status_code status = read_employees(in, &emps, &cnt);
    fclose(in);
    printf(errors[status]);
    if (status)
    {
        fclose(out);
        return status;
    }

    switch (argv[1][1])
    {
    case 'a':
        qsort((void*) emps, cnt, sizeof(Employee*), ascend_comparator);
        break;
    case 'd':
        qsort((void*) emps, cnt, sizeof(Employee*), descend_comparator);
        break;
    
    default:
        for(int i = 0; i < cnt; ++i)
        {
            free(emps[i]->name);
            free(emps[i]->surname);
            free(emps[i]);
        }
        free(emps);
        fclose(out);
        printf(errors[UNKNOWN_ERROR]);
        
        return UNKNOWN_ERROR;
    }
    for(int i = 0; i < cnt-1; ++i)
    {
        fprintf(out, "%llu %s %s %lf\n", (emps[i])->id, (emps[i])->name, (emps[i])->surname, (emps[i])->salary);
        free(emps[i]->name);
        free(emps[i]->surname);
        free(emps[i]);
    }
    fprintf(out, "%llu %s %s %lf", (emps[cnt-1])->id, (emps[cnt-1])->name, (emps[cnt-1])->surname, (emps[cnt-1])->salary);
    free(emps[cnt-1]->name);
    free(emps[cnt-1]->surname);
    free(emps[cnt-1]);
    free(emps);
    fclose(out);

    return OK;
}
