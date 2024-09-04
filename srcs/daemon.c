#include <ft_shield.h>
#include <stdio.h>

int	check_lock_file(void)
{
	int	lfd = open(LOCK_FILE, O_CREAT | O_RDWR, 0666);
	if (lfd == -1)
		return 0;
	if (flock(lfd, LOCK_EX | LOCK_NB) != 0)
	{
		close(lfd);
		return 0;
	}
	return lfd;
}

void	remove_lock_file(int fd)
{
	flock(fd, LOCK_UN);
	close(fd);
	remove(LOCK_FILE);
}

void	skeleton_daemon(void)
{
	pid_t   pid;

	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);
	if (setsid() < 0)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);
	umask(0);
	chdir("/");
}
