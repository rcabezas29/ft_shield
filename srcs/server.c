#include <ft_shield.h>

static int setup_server(void)
{
    int server_socket;
    struct sockaddr_in server_address;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)))
        exit(EXIT_FAILURE);
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

static void _handle_connection(int server_socket, int *n_clients, struct pollfd *pfds)
{
    struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
    int client_fd;

    if ((client_fd = accept(server_socket, (struct sockaddr *)&client, &client_len)))
        return ;
    if (*n_clients >= MAX_CLIENTS)
    {
        send(client_fd, "ft_shield: Connection refused\r\n", 32, 0);
        close(client_fd);
        return ;
    }
    pfds[*n_clients + 1] = (struct pollfd){client_fd, POLLIN, 0};
    (*n_clients)++;
}

static void _handle_client_input()
{

}

static void loop_server(int server_socket)
{
    struct pollfd pfds[MAX_CLIENTS + 1];
    int n_clients;

    n_clients = 0;
    pfds[0] = (struct pollfd){server_socket, POLLIN, 0};
    while (1)
    {
        if (poll(pfds, n_clients + 1, -1) == -1)
            break ;
        for (int i = 0; i < n_clients + 1; i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (pfds[i].fd == server_socket)
                    _handle_connection(server_socket, &n_clients, pfds);
                else
                    _handle_client_input();
            }
        }
    }
}

void server(void)
{
    int server_socket;

    server_socket = setup_server();
    loop_server(server_socket);

}