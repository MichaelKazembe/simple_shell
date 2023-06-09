#include "shell.h"

/**
 * main - The entry point of the program that executes commands from the terminal.
 *
 * @param ac The number of arguments passed to the program.
 * @param av The array of arguments passed to the program.
 *
 * @return 0 upon successful completion of the program, or another integer if desired.
 */
int main(__attribute__((unused))int ac, char **av)
{	
	int status = 0;
	int line_nm = 1;
	char *shell_name = NULL;

	int bytes_read;
	int is_separated = FALSE;
	int i;
	size_t buf_size = 1;
	char *buf = NULL;
	char *buf_ptr;
	char *buf_tmp;
	char **args = NULL;

	shell_name = _strdup(*av);

	environ = arr_cpy(environ, list_length(environ, NULL));

	signal(SIGINT, SIG_IGN);

	buf = malloc(1);
	if (buf == NULL)
		exit(EXIT_FAILURE);

	while (1)
	{
		if (is_separated == FALSE)
		{
			if (isatty(STDIN_FILENO) == 1)
				write(STDOUT_FILENO, "SMshell$ ", 10);

			bytes_read = getline(&buf, &buf_size, stdin);

			if (bytes_read == -1)
				break;
			if (bytes_read == 1)
			{
				line_nm++;
				continue;
			}
			buf[bytes_read - 1] = '\0';
			buf = input_sanitizer(buf, &buf_size);
			if (buf_size == 0)
			{
				line_nm++;
				continue;
			}
			buf_ptr = buf;
		}
		else
			buf_ptr = buf_tmp;

		buf_tmp = NULL;
		args = make_arr(buf_ptr, ' ', &buf_tmp);
		if (buf_tmp != NULL)
			is_separated = TRUE;
		else
			is_separated = FALSE;

		i = cmd_manager(args);

		free(args);

		if (is_separated == FALSE)
			line_nm++;

		if (i == EXT_SHELL)
			break;
	}
	free(buf);
	alias_fun(NULL, TRUE);
	free_arr(environ);
	free(shell_name);

	return (status % 256);
}

