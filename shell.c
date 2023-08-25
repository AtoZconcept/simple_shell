#include "shell.h"

/**
 * main - Simple shell entry point
 * @argc: Argument counter
 * @argv: Argument Vector
 * Return: Errno
 */

int main(int argc, char **argv)
{
	int mode = isatty(0);
	char *buffer = NULL, cmdc, **args = NULL, *fullcmd = NULL;
	size_t buff_Size = 0;
	ssize_t num = 0;
	int count = 0, builtin_status = 0;
	errno = 0;

	(void)argc;
	while (1)
	{
		count++;
		if (mode == 1)
			write(1, "$ ", 2);

		num = getline(&buffer, &buff_Size, stdin);
		if (num == -1)
		{
			free(buffer);
			exit(errno);
		}
		fix(buffer);
		args = tokens(buffer);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}
		if (access(args[0], X_OK) == -1)
		{
			builtin_status = builtin(args, argv[0], buffer);
			if (builtin_status == 1)
				continue;
			fullcmd = full_path(path(), args[0]);
			if (fullcmd == NULL)
			{
				cmdc = (count + '0');
				zerror(argv[0], cmdc, args[0]);
				free(args);
				errno = 127;
				continue;
			}
			ex_exec(args, argv, fullcmd);
			continue;
		}
		exec(args, argv);
	}
	return (errno);
}
