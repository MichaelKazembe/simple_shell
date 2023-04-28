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
<<<<<<< HEAD

	int read_btyes;
	int sep = FALSE;
=======
	
	int bytes_read;
	int is_separated = FALSE;
>>>>>>> parent of a71c1e4...  removed global variables in files
	int i;
	size_t bufferfer_s = 1;
	char *buffer = NULL;
	char *buffer_ptr;
	char *buffer_tmp;
	char **args = NULL;

	shell_name = _strdup(*av);

	environ = arr_cpy(environ, list_length(environ, NULL));

	signal(SIGINT, SIG_IGN);

	buffer = malloc(1);
	if (buffer == NULL)
		exit(EXIT_FAILURE);

	while (1)
	{
		if (sep == FALSE)
		{
			if (isatty(STDIN_FILENO) == 1)
				write(STDOUT_FILENO, "SMshell$ ", 10);

			read_btyes = getline(&buffer, &bufferfer_s, stdin);

			if (read_btyes == -1)
				break;
			if (read_btyes == 1)
			{
				line_nm++;
				continue;
			}
			buffer[read_btyes - 1] = '\0';
			buffer = input_sanitizer(buffer, &bufferfer_s);
			if (bufferfer_s == 0)
			{
				line_nm++;
				continue;
			}
			buffer_ptr = buffer;
		}
		else
			buffer_ptr = buffer_tmp;

		buffer_tmp = NULL;
		args = make_arr(buffer_ptr, ' ', &buffer_tmp);
		if (buffer_tmp != NULL)
			sep = TRUE;
		else
			sep = FALSE;

		i = cmd_manager(args);

		free(args);

		if (sep == FALSE)
			line_nm++;

		if (i == EXT_SHELL)
			break;
	}
	free(buffer);
	alias_fun(NULL, TRUE);
	free_arr(environ);
	free(shell_name);

	return (status % 256);
}

