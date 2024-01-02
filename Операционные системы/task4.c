#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define ull unsigned long long
#define ll long long

enum status_code
{
  ok,
  invalid_input,
  overflow,
  malloc_error,
  file_opening_error,
  file_reading_error,
  file_writing_error,
  wrong_way_error,
  process_creating_error,
  unknown_error
};

static const char* errors[] =
{
  "Success\n",
  "Invalid input\n",
  "Overflow error\n",
  "Memory allocation error\n",
  "Cannot open file\n",
  "Cannot read file correctly\n",
  "Writing to file error\n",
  "Wrong way to file\n",
  "Error of creating a new process\n"
  "Unknown error\n"
};

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

int search_str(FILE* input, char* str, int* ans)
{
  if (input == NULL || str == NULL || ans == NULL)
  {
    return invalid_input;
  }
  int len = strlen(str);
  if (len == 0) return invalid_input;
  
  char* buf = calloc(len+1, sizeof(char));

  if (buf == NULL)
  {
    return malloc_error;
  }
  buf[len] = '\0';

  char c;
  for (int i = 0; i < len; ++i)
  {
    c = getc(input);
    if (c == EOF)
    {
      *ans = 0;
      return ok;
    }
    buf[i] = c;
  }
  if (!strcmp(buf, str))
  {
    *ans = 1;
    return ok;
  }
  int buf_end = len-1;
  c = getc(input);
  while (c != EOF)
  {
    if(++buf_end == len) buf_end = 0;
    buf[buf_end] = c;

    if (c == str[len-1])
    {
      int i;
      for(i = buf_end + len; i > buf_end; --i)
      {
        if (buf[i%len] != str[i-buf_end-1]) break;
      }
      if (i == buf_end)
      {
        *ans = 1;
        free(buf);
        return ok;
      }
    }
    c = getc(input);
  }

  free(buf);
  *ans = 0;
  return ok;
}

int search_paths(FILE* input, int* ans_len, char*** ans)
{
  if (input == NULL || ans_len == NULL || ans == NULL)
  {
    return invalid_input;
  }
  
  int len, path_end, is_eof = 0;
  char* path;
  FILE* current_file; 
  *ans_len = 0;
  do
  {
    len = 10;
    path = calloc(len, sizeof(char));
    path_end = 0;
    if (path == NULL) return malloc_error;
    path[path_end] = getc(input);

    while (path[path_end] != '\n')
    {
      if ((path_end+1) == len)
      {
        path = (char*) realloc(path, len * 2);
        if (path == NULL)
        {
          return malloc_error;
        }
        len <<= 1;

        if (len > len * 2) 
        {
          free(path);
          return overflow;
        }
        if (path == NULL) 
        {
          free(path);
          return malloc_error;
        }
      }
      path[++path_end] = getc(input);
      if (path[path_end] == EOF)
      {
        is_eof = 1;
        break;
      }
    }
    path[path_end] = '\0';
    path = (char*) realloc(path, path_end);
    *ans = (char**) realloc(*ans, *ans_len);

    if (path == NULL || *ans == NULL) return malloc_error;
    (*ans)[(*ans_len)++] = path;
  }while (!is_eof);

  return ok;
}

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("Usage: <file> <word>\n");
    return ok;
  }
  if (argc != 3)
  {
    printf("%d\n", argc);
    printf("%s", errors[invalid_input]);
    return invalid_input;
  }

  FILE* input = fopen(argv[1], "r");
  if (input == NULL)
  {
    printf("%s", errors[file_opening_error]);
    return file_opening_error;
  }
  char** paths;
  int file_number;
  int status = search_paths(input, &file_number, &paths);
  fclose(input);
  if (status != ok) 
  {
    for(int i = 0; i < file_number; ++i)
    {
      free(paths[i]);
    }
    free(paths);
    printf("%s", errors[status]);
    return status;
  }

  pid_t pid;
  for (int i = 0; i < file_number; ++i)
  {
    pid = fork();
    if (pid == 0)
    {
      int found;
      input = fopen(paths[i], "r");
      if (input == NULL) 
      {
        printf("The path to file on line %d is incorrect\n", i+1);
        break;
      }
      status = search_str(input, argv[2], &found);
      if (status == ok)
      {
        if (found)
        {
          printf("The string is contained in the file on line %d\n", i+1);
        }
        else
        {
          printf("The string wasn't found in the file on line %d\n", i+1);
        }
      }
      else
      {
        printf("While processing the file on line %d something went wrong:\n", i+1);
        printf("%s", errors[status]);
      }
      fclose(input);
      break;
    }
    else if (pid == -1)
    {
      printf("%s", errors[process_creating_error]);
      return process_creating_error;
    }
  }
  return ok;
}