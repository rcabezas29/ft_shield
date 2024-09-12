#include <ft_shield.h>

static void remove_client(t_client *clients, int *n_clients, int client_fd)
{
    dprintf(2, "REMOVING: %d\n", client_fd);
    close(client_fd);
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        if (clients[i].pfd.fd == client_fd) {
            clients[i].pfd.fd = -1;
            clients[i].logged = false;
            break;
        }
    }
    (*n_clients)--;
    dprintf(2, "LESS_CLIENT: %d -> %d\n", client_fd, *n_clients);
}

void _handle_client_input(t_client *clients, int *n_clients, int client_fd)
{
    int len_read;
	char buffer[BUFFER_SIZE];

	bzero(&buffer, BUFFER_SIZE);
	if ((len_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) <= 0) {
        remove_client(clients, n_clients, client_fd);
        return ;
    }
    dprintf(2, "FROM_CLIENT: %d -> %s\n", *n_clients, buffer);

}

static void _handle_connection(t_client *clients, int *n_clients)
{
    struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
    int client_fd;

    if ((client_fd = accept(clients[0].pfd.fd, (struct sockaddr *)&client, &client_len)) == -1)
        return ;
    dprintf(2, "CLIENTS: %d\n", *n_clients);
    if (*n_clients >= MAX_CLIENTS)
    {
        send(client_fd, "ft_shield: Connection refused\r\n", 32, 0);
        close(client_fd);
        return ;
    }
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        if (clients[i].pfd.fd == -1) {
            clients[i] = (t_client){(struct pollfd){client_fd, POLLIN, 0}, 0};
            break;
        }
    }
    (*n_clients)++;
    dprintf(2, "NEW_CLIENT: %d -> %d\n", client_fd, *n_clients);
    // if (send(client_fd, "Password: ", 10, 0) == -1)
        // remove_client(clients, n_clients, client_fd);
}

void loop_server(t_client *clients, int server_socket)
{
    int n_clients;

    n_clients = 0;
    clients[0] = (t_client){(struct pollfd){server_socket, POLLIN, 0}, 1};
    while (1)
    {
        if (poll((struct pollfd *)clients, n_clients + 1, -1) == -1)
            break ;
        for (int i = 0; i < n_clients + 1; i++)
        {
            if (clients[i].pfd.revents & POLLIN)
            {
                if (clients[i].pfd.fd == server_socket) {
                    _handle_connection(clients, &n_clients);
                }
                else
                    _handle_client_input(clients, &n_clients, clients[i].pfd.fd);
            }
            else if (clients[i].pfd.revents & POLLHUP)
                remove_client(clients, &n_clients, clients[i].pfd.fd);
        }
    }
}

static int setup_server(void)
{
    int server_socket;
    struct sockaddr_in server_address;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(EXIT_FAILURE);

    int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(server_socket);
		exit(EXIT_FAILURE);
	}
    server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(server_socket);
		exit(EXIT_FAILURE);
	}
    if (listen(server_socket, MAX_CLIENTS) == -1)
	{
		close(server_socket);
		exit(EXIT_FAILURE);
	}
    return server_socket;
}

static void reset_clients(t_client *clients)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++)
    {
        clients[i].pfd.fd = -1;
        clients[i].pfd.events = 0;
        clients[i].logged = false;
    }
}

void server(void)
{
    int server_socket;
    t_client clients[MAX_CLIENTS + 1];

    reset_clients(clients);
    server_socket = setup_server();
    loop_server(clients, server_socket);
}
