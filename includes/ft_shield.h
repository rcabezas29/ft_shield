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
#include <sys/poll.h>
#include <stdio.h>

/*
	DAEMON
*/

void	skeleton_daemon(void);
void	copy_binary(void);
