#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

int ctoi(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'z') return c - 'a';
	if (c >= 'A' && c <= 'Z') return c - 'A';
	return -1; 
}

status_code htable_set_null(Hash_table* ht)
{
	if (ht == NULL) return INVALID_PARAMS;
	ht->chains = NULL;
	ht->chain_sizes = NULL;
	ht->cache = NULL;
	return OK;
}

status_code htable_construct(Hash_table* ht, ull (*calc_hash)(const char*, ull))
{
	if (ht == NULL) return INVALID_PARAMS;
	
	ht->chains = (Ht_node**) calloc(MIN_HT_SIZE, sizeof(Ht_node**));
	if (ht->chains == NULL) 
    {
		ht->chain_sizes = NULL;
		ht->cache = NULL;
        return ALLOC_ERROR;
    }
    ht->chain_sizes = (ull*) calloc(MIN_HT_SIZE, sizeof(ull));
    if (ht->chain_sizes == NULL)
    {
        free(ht->chains);
        ht->chains = NULL;
		ht->cache = NULL;
        return ALLOC_ERROR;
    }
	ht->cache = (Ht_cache*) malloc(sizeof(Ht_cache) * 4);
    if (ht->cache == NULL)
    {
        free(ht->chains);
        free(ht->chain_sizes);
		ht->chains = NULL;
		ht->chain_sizes = NULL;
        return ALLOC_ERROR;
    }
	ht->calc_hash = calc_hash;
	ht->ht_size = MIN_HT_SIZE;
	ht->max_chain_size = ht->min_chain_size = 0;
	ht->min_chain_cnt = MIN_HT_SIZE;
	ht->cache_cnt = 0;
	ht->cache_size = 4;
	return OK;
}

status_code htable_destruct(Hash_table* ht)
{
	if (ht == NULL) return INVALID_PARAMS;
	
	for (ull i = 0; i < ht->cache_cnt; ++i)
	{
		free(ht->cache[i].node->key);
        free(ht->cache[i].node->value);
        free(ht->cache[i].node);
	}
	free(ht->chains);
    free(ht->chain_sizes);
    free(ht->cache);
	ht->chains = NULL;
	ht->chain_sizes = NULL;
	ht->cache = NULL;
	return OK;
}

status_code htable_rebuild(Hash_table* ht, ull param)
{
    ull new_size;
    Ht_node** new_chains;
    ull* new_chain_sizes;
    ull ind;
    Ht_node* node;
	ull max_chain_size = 0;
	ull min_chain_size = ULLONG_MAX;
	ull min_chain_cnt = 0;
	if (ht == NULL) return INVALID_PARAMS;
	
	if (ht->ht_size > MAX_HT_SIZE / param) return ALLOC_ERROR;
	
	new_size = ht->ht_size * param;
	new_chains = (Ht_node**) calloc(new_size, sizeof(Ht_node*));
    if (new_chains == NULL) return ALLOC_ERROR;
	new_chain_sizes = (ull*) calloc(new_size, sizeof(ull));
    if (new_chain_sizes == NULL) 
    {
        free(new_chains);
        return ALLOC_ERROR;
    }
	for (ull i = 0; i < ht->cache_cnt; ++i)
	{
		ind = ht->cache[i].big_hash % new_size;
		node = ht->cache[i].node;
		node->next = new_chains[ind];
		new_chains[ind] = node->next;
		new_chain_sizes[ind]++;
	}
	max_chain_size = 0;
	min_chain_size = ULLONG_MAX;
	min_chain_cnt = 0;
	for (ull i = 0; i < new_size; ++i)
	{
		if (new_chain_sizes[i] < min_chain_size)
		{
			min_chain_size = new_chain_sizes[i];
			min_chain_cnt = 1;
		}
		else if (new_chain_sizes[i] == min_chain_size)
		{
			min_chain_cnt++;
		}
		if (new_chain_sizes[i] > max_chain_size)
		{
			max_chain_size = new_chain_sizes[i];
		}
	}
	if (max_chain_size > 2 * (min_chain_size ? min_chain_size : 1))
	{
		free(new_chains);
        free(new_chain_sizes);
		return htable_rebuild(ht, param * 2);
	}
	free(ht->chains); 
    free(ht->chain_sizes);
	ht->ht_size = new_size;
	ht->chains = new_chains;
	ht->chain_sizes = new_chain_sizes;
	ht->max_chain_size = max_chain_size;
	ht->min_chain_size = min_chain_size;
	ht->min_chain_cnt = min_chain_cnt;
	return OK;
}

status_code htable_search(Hash_table* ht, const char* key, int create_flag, Ht_node** node)
{
    ull hash;
    ull ind;
    Ht_node* cur;
    Ht_cache* tmp;
    Ht_node* new_node;
    char* key_copy;
    ull cur_size;
    ull mn;
    status_code status;
	if (ht == NULL || key == NULL || node == NULL) INVALID_PARAMS;
	hash = ht->calc_hash(key, (ull) 1 << 58);
	ind = hash % ht->ht_size;
	cur = ht->chains[ind];
	while (cur != NULL && strcmp(cur->key, key))
	{
		cur = cur->next;
	}
	if (cur != NULL || !create_flag)
	{
		*node = cur;
		return OK;
	}
	
	if (ht->cache_cnt == ht->cache_size)
	{
		tmp = (Ht_cache*) realloc(ht->cache, sizeof(Hash_table) * ht->cache_size * 2);
		if (tmp == NULL)
		{
			return ALLOC_ERROR;
		}
		ht->cache_size *= 2;
		ht->cache = tmp;
	}
	
	new_node = (Ht_node*) malloc(sizeof(Ht_node));
    if (new_node == NULL) return ALLOC_ERROR;
	key_copy = (char*) malloc(sizeof(char) * (strlen(key) + 1));
	if (key_copy == NULL)
	{
		free(new_node);
		return ALLOC_ERROR;
	}
	strcpy(key_copy, key);
	new_node->key = key_copy;
	new_node->value = NULL;
	new_node->next = ht->chains[ind];
	ht->chains[ind] = new_node;
	ht->cache[ht->cache_cnt++] = (Ht_cache) { .node = new_node, .big_hash = hash };
	*node = new_node;
	
	cur_size = ht->chain_sizes[ind];
	ht->chain_sizes[ind]++;
	if (ht->max_chain_size == cur_size)
	{
		ht->max_chain_size++;
	}
	if (ht->min_chain_size == cur_size)
	{
		ht->min_chain_cnt--;
		if (ht->min_chain_cnt == 0)
		{
			ht->min_chain_size = cur_size + 1;
			for (ull i = 0; i < ht->ht_size; ++i)
			{
				if (ht->chain_sizes[i] == ht->min_chain_size)
				{
					ht->min_chain_cnt++;
				}
			}
		}
	}
	mn = ht->min_chain_size ? ht->min_chain_size : 1;
	if (ht->max_chain_size > 2 * mn)
	{
		status = htable_rebuild(ht, 2);
		return status;
	}
	return OK;
}

status_code htable_contains(Hash_table* ht, const char* key, int* is_contained)
{
    Ht_node* node = NULL;
    status_code status;
	if (ht == NULL || key == NULL || is_contained == NULL) return INVALID_PARAMS;
	status = htable_search(ht, key, 0, &node);
	*is_contained = node ? 1 : 0;
	return status;
}

status_code htable_set_value(Hash_table* ht, const char* key, const char* value)
{
    Ht_node* node = NULL;
    status_code status;
	if (ht == NULL || key == NULL || value == NULL) return INVALID_PARAMS;
	status = htable_search(ht, key, 1, &node);
	if (status) return status;
	free(node->value);
	node->value = (char*) malloc(sizeof(char) * (strlen(value) + 1));
	if (node->value == NULL) return ALLOC_ERROR;
	strcpy(node->value, value);
	return OK;
}

status_code htable_get_value(Hash_table* ht, const char* key, char** value)
{
    Ht_node* node = NULL;
    status_code status;
	if (ht == NULL || key == NULL || value == NULL) INVALID_PARAMS;
	status = htable_search(ht, key, 0, &node);
	if (status) return status;
	if (node == NULL) return ACCESS_ERROR;

	*value = (char*) malloc(sizeof(char) * (strlen(node->value) + 1));
	if (*value == NULL) return ALLOC_ERROR;
	strcpy(*value, node->value);
	return OK;
}

ull calc_default_hash(const char* str, ull mod)
{
	ull res = 0;
	if (str == NULL)
	{
		return 0;
	}
	for (int i = 0; str[i]; ++i)
	{
		res = res * KEY_ALPHABET_SIZE % mod;
		res = (res + ctoi(str[i])) % mod;
	}
	return res;
}

int is_separator(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

status_code fskip_separators(FILE* in)
{
	char c = ' ';
	if (in == NULL) return INVALID_PARAMS;
	if (feof(in)) return OK;
	while (is_separator(c))
	{
		c = fgetc(in);
	}
	ungetc(c, in);
	return OK;
}

status_code fread_word(FILE* in, char** word)
{
	char c;
	ull size = 2;
	char* tmp;
	ull i = 0;
	if (in == NULL || word == NULL || feof(in)) return INVALID_PARAMS;
	*word = (char*) malloc(sizeof(char) * size);
	if (*word == NULL) return ALLOC_ERROR;
	c = fgetc(in);
	while (!is_separator(c) && !feof(in))
	{
		if (i + 2 == size)
		{
			size *= 2;
			tmp = (char*) realloc(*word, sizeof(char) * size);
			if (tmp == NULL)
			{
				free(*word);
				*word = NULL;
				return ALLOC_ERROR;
			}
			*word = tmp;
		}
		(*word)[i++] = c;
		c = fgetc(in);
	}
	(*word)[i] = '\0';
	tmp = (char*) realloc(*word, sizeof(char) * (i+1));
	if (tmp == NULL)
	{
		free(*word);
		*word = NULL;
		return ALLOC_ERROR;
	}
	*word = tmp;
	return OK;
}

status_code fread_line(FILE* in, char** line)
{
	char c;
	ull size = 2;
	char* tmp;
	ull i = 0;
	if (in == NULL || line == NULL || feof(in)) return INVALID_PARAMS;
	*line = (char*) malloc(sizeof(char) * size);
	if (*line == NULL) return ALLOC_ERROR;
	c = fgetc(in);
	while (c != '\n' && !feof(in))
	{
		if (i + 2 == size)
		{
			size *= 2;
			tmp = (char*) realloc(*line, sizeof(char) * size);
			if (tmp == NULL)
			{
				free(*line);
				*line = NULL;
				return ALLOC_ERROR;
			}
			*line = tmp;
		}
		(*line)[i++] = c;
		c = fgetc(in);
	}
	(*line)[i] = '\0';
	tmp = (char*) realloc(*line, sizeof(char) * (i+1));
	if (tmp == NULL)
	{
		free(*line);
		*line = NULL;
		return ALLOC_ERROR;
	}
	*line = tmp;
	return OK;
}

int main(int argc, char** argv)
{
	FILE *input, *output;
	Hash_table macro;
	status_code status;
	char *word, *key, *value;
	char c;
	int is_contained;
	if (argc == 1)
	{
		printf("Usage: cmd_path <input> <output>\n");
		return OK;
	}
	if (argc != 3 || !strcmp(argv[1], argv[2]))
	{
		printf(errors[INVALID_INPUT]);
		return INVALID_INPUT;
	}
	
	input = fopen(argv[1], "r");
	if (input == NULL)
	{
		printf(errors[FILE_OPENING_ERROR]);
		return FILE_OPENING_ERROR;
	}
	output = fopen(argv[2], "w");
	if (input == NULL)
	{
		fclose(input);
		printf(errors[FILE_OPENING_ERROR]);
		return FILE_OPENING_ERROR;
	}
	
	htable_set_null(&macro);
	status = htable_construct(&macro, calc_default_hash);
	word = NULL;
	while (!status)
	{
		key = NULL;
		value = NULL;
		status = status ? status : fread_word(input, &word);
		if (!status && !strcmp(word, "#define"))
		{
			free(word);
			status = status ? status : fskip_separators(input);
			status = status ? status : fread_word(input, &key);
			status = status ? status : fskip_separators(input);
			status = status ? status : fread_line(input, &value);
			status = status ? status : htable_set_value(&macro, key, value);
		}
		else
		{
			status = RESERVED;
		}
		free(key);
		free(value);
	}
	if (status == RESERVED) status = OK;
	fseek(input, -1, SEEK_CUR);
	c = getc(input);
	if (is_separator(c))
	{
		ungetc(c, input);
		fseek(input, -strlen(word), SEEK_CUR);
	}
	else
	{
		fseek(input, -strlen(word), SEEK_CUR);
	}
	free(word);
	word = NULL;
	
	while (!status)
	{
		c = getc(input);
		while(is_separator(c))
		{
			putc(c, output);
			c = getc(input);
		}
		if (!feof(input))
		{
			fseek(input, -1, SEEK_CUR);
		}
		else status = RESERVED;
		
		status = status ? status : fread_word(input, &word);
		is_contained = 0;
		status = status ? status : htable_contains(&macro, word, &is_contained);
		if (!status && is_contained)
		{
			value = NULL;
			status = htable_get_value(&macro, word, &value);
			fprintf(output, "%s", value);
			free(value);
		}
		else if (!status)
		{
			fprintf(output, "%s", word);
		}
		if (!status)
		{
			fseek(input, -1, SEEK_CUR);
			c = getc(input);
			if (is_separator(c))
			{
				fprintf(output, "%c", c);
			}
		}
		free(word);
		word = NULL;
	}
	if (status == RESERVED) status = OK;
	
	htable_destruct(&macro);
	fclose(input);
	fclose(output);
	printf(errors[status]);
	return status;
}