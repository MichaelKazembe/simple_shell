#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>

#define FALSE 0
#define TRUE 1
#define NETHR 2
#define MARK 3
#define PRFX 4
#define EXT_SHELL 5
#define SKP_FORK 6
#define _EXECVE 7


extern char **environ;

extern int status;

extern int line_nm;

extern char *SMshell;

/**
 * struct Alias - A node in a singly linked list representing an alias.
 *
 * @param name The name of the alias.
 * @param value The command that the alias represents.
 * @param next A pointer to the next node in the list.
 */

typedef struct Alias
{
	char *name;
	char *value;
	struct Alias *next;
} alias;

int cmd_manager(char **args);

int builtins(char **args);

int andor(char **args, char operator, int last_compare);

char *check_cmd(char **args);

int execute_cmd(char **args);

char *input_sanitizer(char *old_buf, size_t *old_size);

int input_err_checking(char *ptr);

void err_msg(char *arg0, char *arg1);

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

int _getline(char **line_ptr, size_t *n, int file);

char *var_check(char *arg);

int _strlen(char *str);

char *_strdup(char *src);

char *str_concat(char *s1, char *s2);

int str_comp(char *s1, char *s2, int pref_or_match);

char *get_arr_element(char **array, char *element_name);

char **make_arr(char *str, char delim, char **if_sep);

int list_length(char **list, char *entry);

char **arr_cpy(char **old_array, int new_size);

int free_arr(char **args);

int _setenv(const char *name, const char *value);

int _unsetenv(const char *name);

int change_directory(char *name);

int alias_fun(char **args, int free);

int free_alias(alias *alias_ptr);

int if_alias(char **args, alias *alias_ptr);

int alias_print(alias *alias_ptr);

int alias_value_print(char *arg, alias *alias_ptr);

int alias_value_set(char *arg, alias *alias_ptr, char *new_value);

int print_environ(void);

char *ito_str(int n);

int _atoi(char *s);


#endif
