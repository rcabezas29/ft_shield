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

#define LOCK_FOLDER "/var/lock/"
#define LOCK_FILE "ft_shield.lock"

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
