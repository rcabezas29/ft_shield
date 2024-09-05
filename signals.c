#include <ft_shield.h>

int	create_signal_fd(void)
{
	int	sfd = setup_signal_fd();
	if (sfd == -1)
		exit(EXIT_FAILURE);
	return sfd;
}

void	handle_signals(struct pollfd *pfds, int *shutdown)
{
	struct signalfd_siginfo	fdsi;
	ssize_t	s = read(pfds[1].fd, &fdsi, sizeof(fdsi));

	if (s != sizeof(struct signalfd_siginfo))
		exit(1);
	if (fdsi.ssi_signo == SIGHUP)
		remove_clients(pfds);
	else if (fdsi.ssi_signo == SIGTERM || fdsi.ssi_signo == SIGINT || fdsi.ssi_signo == SIGQUIT)
		*shutdown = 1;
}

int	setup_signal_fd(void)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
	{
        perror("sigprocmask");
        return -1;
    }
    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        return -1;
    }
    return sfd;
}