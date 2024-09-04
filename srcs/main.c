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
	int	lfd;

	write(1, "rorozco- & rcabezas\n", 20);
	if (getuid()) // Works only with root permissions
		return EXIT_FAILURE;
	int	exec_mode;
	switch (exec_mode = executed_mode())
	{
		case 1: // Executed as root - Create service
			write(1, "Executed as root user\n", 23);
			copy_binary();
			skeleton_daemon();
			break ;
		case 0: // Running as service - Starts server
			write(1, "Executed as daemon\n", 20);
			lfd = check_lock_file();
			if (lfd == 0)
				return EXIT_FAILURE;
			remove_lock_file(lfd);
			// start_server();
			break ;
		case -1: // Error
			write(1, "Error of readlink\n", 19);
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
