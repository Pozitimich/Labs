#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>

typedef long long ll;
typedef unsigned long long ull;

typedef enum
{
	OK,
	INVALID_PARAMS,
	INVALID_INPUT,
	INVALID_FLAG,
	INVALID_NUMBER,
	FILE_OPENING_ERROR,
	FILE_CONTENT_ERROR,
	OVERFLOW_ERROR,
	UNINITIALIZED_USAGE,
	DIVISION_BY_ZERO,
	ALLOC_ERROR,
	CORRUPTED_MEMORY
} status_code;

void print_error(status_code code)
{
	switch (code)
	{
		case OK:
			return;
		case INVALID_PARAMS:
			printf("Invalid function argument\n");
			return;
		case INVALID_INPUT:
			printf("Invalid input\n");
			return;
		case INVALID_FLAG:
			printf("Invalid flag\n");
			return;
		case INVALID_NUMBER:
			printf("Invalid number\n");
			return;
		case FILE_OPENING_ERROR:
			printf("File cannot be opened\n");
			return;
		case FILE_CONTENT_ERROR:
			printf("Invalid content of file\n");
			return;
		case OVERFLOW_ERROR:
			printf("An overflow occurred\n");
			return;
		case UNINITIALIZED_USAGE:
			printf("Uninitialized variable was used\n");
			return;
		case DIVISION_BY_ZERO:
			printf("Division by zero occurred\n");
			return;
		case ALLOC_ERROR:
			printf("Memory lack error occurred\n");
			return;
		case CORRUPTED_MEMORY:
			printf("Memory was corrupted\n");
			return;
		default:
			printf("Unexpected error occurred\n");
			return;
	}
}

typedef enum
{
	NONE,
	VAR,
	L_BRACKET,
	R_BRACKET,
	CONST,
	CONJUCTION,
	DISJUCTION,
	INVERSION,
	IMPLICATION,
	COIMPLICATION,
	EXCLUSIVE,
	EQUIVALENCE,
	SHEFFER_STROKE,
	WEBB_FUNCTION
} node_content;

typedef struct Tree_node
{
	node_content content;
	char data;
	struct Tree_node* left;
	struct Tree_node* right;
} Tree_node;

status_code tree_node_dynamic_construct(Tree_node** node, node_content content, char data)
{
	if (node == NULL)
	{
		return INVALID_PARAMS;
	}
	*node = (Tree_node*) malloc(sizeof(Tree_node));
	if (*node == NULL)
	{
		return ALLOC_ERROR;
	}
	(*node)->content = content;
	(*node)->data = data;
	(*node)->left = (*node)->right = NULL;
	return OK;
}

status_code tree_node_destruct(Tree_node* node)
{
	if (node == NULL)
	{
		return INVALID_PARAMS;
	}
	if (node->left != NULL)
	{
		tree_node_destruct(node->left);
		free(node->left);
	}
	if (node->right != NULL)
	{
		tree_node_destruct(node->right);
		free(node->right);
	}
	return OK;
}

typedef struct stack_node
{
	Tree_node* node;
	struct stack_node* next;
} stack_node;

typedef struct
{
	stack_node* top;
	stack_node* bottom;
	ull size;
} Stack;

status_code stack_set_null(Stack* stack)
{
	if (stack == NULL)
	{
		return INVALID_PARAMS;
	}
	stack->top = stack->bottom = NULL;
	stack->size = 0;
	return OK;
}

status_code stack_construct(Stack* stack)
{
	stack_node* terminator;
    if (stack == NULL)
	{
		return INVALID_PARAMS;
	}
	terminator = (stack_node*) malloc(sizeof(stack_node));
	if (terminator == NULL)
	{
		return ALLOC_ERROR;
	}
	terminator->node = NULL;
	terminator->next = terminator;
	stack->top = stack->bottom = terminator;
	stack->size = 0;
	return OK;
}

status_code stack_destruct(Stack* stack)
{
	stack_node* cur, *tmp;
    if (stack == NULL)
	{
		return INVALID_PARAMS;
	}
	cur = stack->top;
	while (cur != stack->bottom)
	{
		tmp = cur;
		cur = cur->next;
		tree_node_destruct(tmp->node);
		free(tmp->node);
		free(tmp);
	}
	free(cur);
	stack->top = stack->bottom = NULL;
	stack->size = 0;
	return OK;
}

status_code stack_empty(Stack* stack, int* empty)
{
	if (stack == NULL || empty == NULL)
	{
		return INVALID_PARAMS;
	}
	*empty = stack->top == stack->bottom;
	return OK;
}

status_code stack_top(Stack stack, Tree_node** node)
{
	if (node == NULL)
	{
		return INVALID_PARAMS;
	}
	*node = stack.top->node;
	return OK;
}

status_code stack_push(Stack* stack, Tree_node* tr_node)
{
	stack_node* st_node;
    if (stack == NULL)
	{
		return INVALID_PARAMS;
	}
	st_node = (stack_node*) malloc(sizeof(stack_node));
	if (st_node == NULL)
	{
		return ALLOC_ERROR;
	}
	st_node->node = tr_node;
	st_node->next = stack->top;
	stack->top = st_node;
	stack->size++;
	return OK;
}

status_code stack_pop(Stack* stack, Tree_node** node)
{
	stack_node* tmp;
    if (stack == NULL)
	{
		return INVALID_PARAMS;
	}
	if (stack->top == stack->bottom)
	{
		return INVALID_INPUT;
	}
	if (node != NULL)
	{
		*node = stack->top->node;
	}
	tmp = stack->top;
	stack->top = stack->top->next;
	stack->size--;
	free(tmp);
	return OK;
}

typedef struct
{
	Tree_node* root;
	char var_cnt;
	bool var_flag[26];
} Expression_tree;


int is_operation(node_content content)
{
	switch (content)
	{
		case INVERSION:
		case WEBB_FUNCTION:
		case SHEFFER_STROKE:
		case COIMPLICATION:
		case CONJUCTION:
		case DISJUCTION:
		case IMPLICATION:
		case EXCLUSIVE:
		case EQUIVALENCE:
			return 1;
		default:
			return 0;
	}
}

int get_operation_priority(node_content operation)
{
	switch (operation)
	{
		case INVERSION:
			return 3;
		case WEBB_FUNCTION:
		case SHEFFER_STROKE:
		case COIMPLICATION:
		case CONJUCTION:
			return 2;
		case DISJUCTION:
		case IMPLICATION:
		case EXCLUSIVE:
		case EQUIVALENCE:
			return 1;
		default:
			return 0;
	}
}

status_code get_expr_token(const char* src, const char** end_ptr, node_content* content, char* data)
{
	status_code status;
    if (src == NULL || end_ptr == NULL || content == NULL || data == NULL)
	{
		return INVALID_PARAMS;
	}
	status = OK;
	if (isalpha(*src))
	{
		*content = VAR;
		*data = toupper(*src);
	}
    else 
    {
        switch (*src)
        {
        case '1':
        case '0':
            *content = CONST;
            *data = *src;
            break;
        case '&':
            *content = CONJUCTION;
            break;
        case '|':
            *content = DISJUCTION;
            break;
        case '~':
            *content = INVERSION;
            break;
        case '=':
            *content = EQUIVALENCE;
            break;
        case '!':
            *content = SHEFFER_STROKE;
            break;
        case '?':
            *content = WEBB_FUNCTION;
            break;
        case '(':
            *content = L_BRACKET;
            break;
        case ')':
            *content = R_BRACKET;
            break;
        case '-':
            ++src;
            if (*src == '>')
            {
                *content = IMPLICATION;
            }
            else
            {
                status = INVALID_INPUT;
            }
            break;
        case '+':
            ++src;
            if (*src == '>')
            {
                *content = COIMPLICATION;
            }
            else
            {
                status = INVALID_INPUT;
            }
            break;
        case '<':
            ++src;
            if (*src == '>')
            {
                *content = EXCLUSIVE;
            }
            else
            {
                status = INVALID_INPUT;
            }
            break;
        default:
            printf("%c\n", *src);
            status = INVALID_INPUT;
            break;
        }
    }
	if (status)
	{
		*end_ptr = src;
		return status;
	}
	++src;
	*end_ptr = src;
	return OK;
}

status_code validate_token_combination(node_content prev, node_content cur)
{
	if (is_operation(prev) && is_operation(cur) && cur != INVERSION)
	{
		return INVALID_INPUT;
	}
	if ((prev == CONST || prev == VAR) && (cur == CONST || cur == VAR))
	{
		return INVALID_INPUT;
	}
	if ((prev == L_BRACKET && cur == R_BRACKET) || (prev == R_BRACKET && cur == L_BRACKET))
	{
		return INVALID_INPUT;
	}
	if ((is_operation(prev) && cur == R_BRACKET) || (prev == R_BRACKET && cur == INVERSION))
	{
		return INVALID_INPUT;
	}
	if (prev == L_BRACKET && is_operation(cur) && cur != INVERSION)
	{
		return INVALID_INPUT;
	}
	if (((prev == CONST || prev == VAR) && cur == L_BRACKET)
			|| (prev == R_BRACKET && (cur == CONST || cur == VAR)))
	{
		return INVALID_INPUT;
	}
	return OK;
}

status_code expr_tree_set_null(Expression_tree* etree)
{
	etree->root = NULL;
	return OK;
}

status_code expr_tree_destruct(Expression_tree* etree)
{
	if (etree == NULL)
	{
		return INVALID_PARAMS;
	}
	tree_node_destruct(etree->root);
	free(etree->root);
	return OK;
}

status_code expr_tree_construct(Expression_tree* etree, const char* infix)
{
    status_code status;
	Expression_tree etree_tmp;
    ull i;
    Stack args, opers;
	const char* ptr = infix;
    node_content prev_content, content, other_content;
    char data;
    Tree_node* tnode;
    int cur_prior, other_prior, op_arg_cnt; 
	if (etree == NULL || infix == NULL)
	{
		return INVALID_PARAMS;
	}
	
	status = OK;
	etree_tmp.var_cnt = 0;
	for (i = 0; i < 26; ++i)
	{
		etree_tmp.var_flag[i] = false;
	}
	
	expr_tree_set_null(&etree_tmp);
	stack_set_null(&args);
	stack_set_null(&opers);
	status = status ? status : stack_construct(&args);
	status = status ? status : stack_construct(&opers);
	
	prev_content = NONE;
	while (!status && *ptr)
	{
		content = NONE;
		data = 0;
		status = get_expr_token(ptr, &ptr, &content, &data);
		status = status ? status : validate_token_combination(prev_content, content);
		// --- HANDLE VARIABLE AND CONSTANT ---
		if (!status && (content == VAR || content == CONST))
		{
			if (content == VAR)
			{
				if (!etree_tmp.var_flag[data - 'A'])
				{
					etree_tmp.var_cnt++;
				}
				etree_tmp.var_flag[data - 'A'] = 1;
			}
			tnode = NULL;
			status = tree_node_dynamic_construct(&tnode, content, data);
			status = status ? status : stack_push(&args, tnode);
		}
		// --- HANDLE OPERATION ---
		else if (!status && is_operation(content))
		{
			tnode = NULL;
			stack_top(opers, &tnode);
			other_content = tnode != NULL ? tnode->content : NONE;
			cur_prior = get_operation_priority(content);
			other_prior = get_operation_priority(other_content);
			while (!status && other_prior >= cur_prior && content != INVERSION)
			{
				op_arg_cnt = other_content == INVERSION ? 1 : 2;
				status = args.size >= op_arg_cnt ? OK : INVALID_INPUT;
				// combine operation with 1/2 args
				if (op_arg_cnt == 2)
				{
					status = status ? status : stack_pop(&args, &tnode->right);
				}
				status = status ? status : stack_pop(&args, &tnode->left);
				status = status ? status : stack_push(&args, tnode);
				// prepare to check next operation
				status = status ? status : stack_pop(&opers, NULL);
				stack_top(opers, &tnode);
				other_content = tnode != NULL ? tnode->content : NONE;
				other_prior = get_operation_priority(other_content);
			}
			status = status ? status : tree_node_dynamic_construct(&tnode, content, data);
			status = status ? status : stack_push(&opers, tnode);
		}
		// --- HANDLE LEFT BRACKET ---
		else if (!status && content == L_BRACKET)
		{
			tnode = NULL;
			status = tree_node_dynamic_construct(&tnode, content, data);
			status = status ? status : stack_push(&opers, tnode);
		}
		// --- HANDLE RIGHT BRACKET ---
		else if (!status && content == R_BRACKET)
		{
			tnode = NULL;
			stack_top(opers, &tnode);
			status = tnode != NULL ? OK : INVALID_INPUT;
			while (!status && tnode->content != L_BRACKET)
			{
				op_arg_cnt = tnode->content == INVERSION ? 1 : 2;
				status = args.size >= op_arg_cnt ? OK : INVALID_INPUT;
				// combine operation with 1/2 args
				if (op_arg_cnt == 2)
				{
					status = status ? status : stack_pop(&args, &tnode->right);
				}
				status = status ? status : stack_pop(&args, &tnode->left);
				status = status ? status : stack_push(&args, tnode);
				// prepare to check next operation
				status = status ? status : stack_pop(&opers, NULL);
				stack_top(opers, &tnode);
				status = status ? status : (tnode != NULL ? OK : INVALID_INPUT);
			}
			tnode = NULL;
			status = status ? status : stack_pop(&opers, &tnode);
			free(tnode);
		}
		prev_content = content;
	}
	while (!status && opers.size > 0)
	{
		op_arg_cnt;
		tnode = NULL;
		status = stack_pop(&opers, &tnode);
		if (!status)
		{
			op_arg_cnt = tnode->content == INVERSION ? 1 : 2;
		}
		status = status ? status : (tnode->content != L_BRACKET ? OK : INVALID_INPUT);
		status = status ? status : (args.size >= op_arg_cnt ? OK : INVALID_INPUT);
		// combine operation with 1/2 args
		if (op_arg_cnt == 2)
		{
			status = status ? status : stack_pop(&args, &tnode->right);
		}
		status = status ? status : stack_pop(&args, &tnode->left);
		status = status ? status : stack_push(&args, tnode);
		if (status)
		{
			tree_node_destruct(tnode);
			free(tnode);
		}
	}
	if (!status)
	{
		status = args.size == 1 ? OK : INVALID_INPUT;
		tnode = NULL;
		status = status ? status : stack_pop(&args, &tnode);
		etree_tmp.root = tnode;
	}
	stack_destruct(&args);
	stack_destruct(&opers);
	if (status)
	{
		expr_tree_destruct(&etree_tmp);
		return status;
	}
	*etree = etree_tmp;
	memcpy(etree->var_flag, etree_tmp.var_flag, sizeof(bool) * 26);
	return OK;
}

status_code expr_tree_node_calc(const Tree_node* node, const bool var_table[26], bool* res)
{
    status_code status;
	bool res_left, res_right;
	if (res == NULL)
	{
		return INVALID_PARAMS;
	}
	
	switch (node->content)
	{
		case NONE:
		case L_BRACKET:
		case R_BRACKET:
			return INVALID_INPUT;
		case VAR:
			if (node->data - 'A' < 0 || node->data - 'A' > 25)
			{
				return INVALID_INPUT;
			}
			*res = var_table[node->data - 'A'];
			return OK;
		case CONST:
			if (node->data != '0' && node->data != '1')
			{
				return INVALID_INPUT;
			}
			*res = node->data == '1' ? true : false;
			return OK;
        case INVERSION:
            if (node->left == NULL)
            {
                return INVALID_INPUT;
            }
            status = expr_tree_node_calc(node->left, var_table, res);
            *res = status ? *res : !(*res);
		    return status;
		default:
			break;
	}
	if (node->left == NULL || node->right == NULL)
	{
		return INVALID_INPUT;
	}
	status = expr_tree_node_calc(node->left, var_table, &res_left);
	status = status ? status : expr_tree_node_calc(node->right, var_table, &res_right);
	if (status)
	{
		return status;
	}
	
	switch (node->content)
	{
		case CONJUCTION:
			*res = res_left && res_right;
			return OK;
		case DISJUCTION:
			*res = res_left || res_right;
			return OK;
		case IMPLICATION:
			*res = !res_left || res_right;
			return OK;
		case COIMPLICATION:
			*res = res_left && !res_right;
			return OK;
		case EXCLUSIVE:
			*res = res_left ^ res_right;
			return OK;
		case EQUIVALENCE:
			*res = res_left == res_right;
			return OK;
		case SHEFFER_STROKE:
			*res = !(res_left && res_right);
			return OK;
		case WEBB_FUNCTION:
			*res = !(res_left || res_right);
			return OK;
		default:
			return INVALID_INPUT;
	}
}

status_code expr_tree_calc(Expression_tree etree, const bool* var_values, bool* res)
{
	bool var_table[26], res_tmp;
    ull i, j;
    status_code status;
	if (etree.root == NULL || var_values == NULL || res == NULL)
	{
		return INVALID_PARAMS;
	}
	
	for (i = 0, j = 0; i < 26; ++i)
	{
		if (etree.var_flag[i])
		{
			var_table[i] = var_values[j++];
		}
	}
	
	status = expr_tree_node_calc(etree.root, var_table, &res_tmp);
	if (status)
	{
		return status;
	}
	*res = res_tmp;
	return OK;
}

status_code expr_tree_fprint_table(FILE* file, Expression_tree etree)
{
    status_code status = OK;
    bool* var_values;
    bool res;
    ull i, mask_end, var_mask, var_mask2;
	if (etree.root == NULL)
	{
		return INVALID_PARAMS;
	}
	
	var_values = (bool*) malloc(sizeof(bool) * etree.var_cnt);
	if (var_values == NULL)
	{
		return ALLOC_ERROR;
	}
	
	if (etree.var_cnt == 0)
	{
		status = expr_tree_calc(etree, var_values, &res);
		free(var_values);
		if (status)
		{
			return status;
		}
		fprintf(file, "Value of the expression is %d\n", res);
		return OK;
	}
	
	for (i = 0; i < 26; ++i)
	{
		if (etree.var_flag[i])
		{
			fprintf(file, "| %c ", 'A' + (int) i);
		}
	}
	fprintf(file, "| Value |\n");
	
	mask_end = 2 << (etree.var_cnt - 1);
	for (var_mask = 0; var_mask < mask_end; ++var_mask)
	{
		var_mask2 = var_mask;
		for (i = etree.var_cnt; i > 0; --i)
		{
			var_values[i - 1] = var_mask2 & 1;
			var_mask2 >>= 1;
		}
		status = expr_tree_calc(etree, var_values, &res);
		if (status)
		{
			free(var_values);
			return status;
		}
		for (i = 0; i < etree.var_cnt; ++i)
		{
			fprintf(file, "| %d ", var_values[i]);
		}
		fprintf(file, "|   %d   |\n", res);
	}
	free(var_values);
	return OK;
}

status_code fread_line(FILE* file, char** line)
{
	ull iter = 0;
	ull size = 2;
    char c;
    char* temp_line;
	if (file == NULL || line == NULL)
	{
		return INVALID_PARAMS;
	}
	*line = (char*) malloc(sizeof(char) * size);
	if (*line == NULL)
	{
		return ALLOC_ERROR;
	}
	c = getc(file);
	while (!feof(file) && c != '\n')
	{
		if (iter > size - 2)
		{
			size *= 2;
			temp_line = realloc(*line, size);
			if (temp_line == NULL)
			{
				free(*line);
				return ALLOC_ERROR;
			}
			*line = temp_line;
		}
		(*line)[iter++] = c;
		c = getc(file);
	}
	(*line)[iter] = '\0';
	return OK;
}

status_code generate_random_str(char** str)
{
	ull iter = 0;
	ull size = 2;
	char symbols[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char c;
    char* temp_line;
	if (str == NULL)
	{
		return INVALID_PARAMS;
	}
	*str = (char*) malloc(sizeof(char) * size);
	if (*str == NULL)
	{
		return ALLOC_ERROR;
	}
	
	c = symbols[rand() % 62];
	while ((c != '\0' || iter == 0) && iter < 63)
	{
		if (iter > size - 2)
		{
			size *= 2;
			temp_line = realloc(*str, size);
			if (temp_line == NULL)
			{
				free(*str);
				return ALLOC_ERROR;
			}
			*str = temp_line;
		}
		(*str)[iter++] = c;
		c = symbols[rand() % 63];
	}
	(*str)[iter] = '\0';
	return OK;
}

status_code construct_output_path(const char* input, const char* output_name, char** output)
{
	ull last_delim_pos = -1, i;
	if (input == NULL || output_name == NULL || output == NULL)
	{
		return INVALID_PARAMS;
	}
	for (i = 0; input[i]; ++i)
	{
		if (input[i] == '/' || input[i] == '\\')
		{
			last_delim_pos = i;
		}
	}
	*output = (char*) malloc(sizeof(char) * (last_delim_pos + strlen(output_name) + 2));
	if (*output == NULL)
	{
		return ALLOC_ERROR;
	}
	memcpy(*output, input, sizeof(char) * (last_delim_pos + 1));
	strcpy(*output + last_delim_pos + 1, output_name);
	return OK;
}

int main(int argc, char** argv)
{
    status_code status;
    char* output_name, *input_path, *output_path;
    FILE* input, *output;
	char* infix_expr;
	Expression_tree etree;
	if (argc == 1)
	{
		printf("Usage: cmd_path <input>\n");
		return OK;
	}
	if (argc != 2)
	{
		print_error(INVALID_INPUT);
		return INVALID_INPUT;
	}
	
	status = OK;
	srand(time(NULL));
	output_name = NULL;
	input_path = argv[1];
	output_path = NULL;
	
	input = fopen(input_path, "r");
	if (input == NULL)
	{
		print_error(FILE_OPENING_ERROR);
		return FILE_OPENING_ERROR;
	}
	
	while (!status && (output_path == NULL || !strcmp(input_path, output_path)))
	{
		free(output_name);
		free(output_path);
		status = generate_random_str(&output_name);
		status = status ? status : construct_output_path(input_path, output_name, &output_path);
	}
	free(output_name);
	if (status)
	{
		fclose(input);
		print_error(status);
		return status;
	}
	
	output = fopen(output_path, "w");
	if (output == NULL)
	{
		fclose(output);
		free(output_path);
		print_error(FILE_OPENING_ERROR);
		return FILE_OPENING_ERROR;
	}
	printf("Output file path: %s\n", output_path);
	free(output_path);
	
	status = fread_line(input, &infix_expr);
	
	expr_tree_set_null(&etree);
	status = status ? status : expr_tree_construct(&etree, infix_expr);
	status = status ? status : expr_tree_fprint_table(output, etree);
	
	expr_tree_destruct(&etree);
	free(infix_expr);
	fclose(input);
	fclose(output);
	if (status)
	{
		print_error(status);
	}
	return status;
}