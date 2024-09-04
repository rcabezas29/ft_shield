#include <ft_shield.h>

struct pollfd	*init_server(void)
{
	struct pollfd	*pfds = calloc(6, sizeof(struct pollfd));

	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		exit(EXIT_FAILURE);

	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in	server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	if (listen(socket_fd, MAX_CLIENTS) == -1)
	{
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	pfds[0] = (struct pollfd){socket_fd, POLLIN, 0};
	return pfds;
}

void	remove_clients(struct pollfd *pfds)
{
	for (size_t i = 2; i < MAX_CLIENTS + 2; ++i)
	{
		close(pfds[i].fd);
		pfds[i].fd = 0;
	}
}

int		handle_client_input(int client_socket)
{
	(void)client_socket;
	// char	buffer[1024];

	// memset(buffer, 0, sizeof(buffer));
	// ssize_t	bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	// if (bytes_received <= 0)
	// {
	// 	this->remove_client(client_socket);
	// 	close(client_socket);
	// 	return 0;
	// }

	// std::string	message(buffer);
	// message = message.substr(0, bytes_received - 1);
	// message.erase(message.find_last_not_of("\r\n") + 1);
	// if (message == "quit")
	// {
	// 	this->_tintin_reporter->log("Request quit", "INFO");
	// 	this->shutdown_server();
	// 	close(client_socket);
	// 	return -1;
	// }
	// this->_tintin_reporter->log("User input: " + message, "LOG");
	return 0;
}

void	shutdown_server(struct pollfd *pfds, bool *running, int n_clients)
{
	if (*running)
	{
		*running = false;
		for (int i = 0; i < n_clients + 2; ++i)
			close(pfds[i].fd);
	}
}

void	start_server(void)
{
	struct pollfd	*pfds = init_server();
	int				n_clients = 0;
	int				sfd;
	int				shutdown_requested = 0;
	bool			running = true;

	sfd = create_signal_fd();
	pfds[1] = (struct pollfd){sfd, POLLIN, 0};
	while (running)
	{
		int pcount = poll(pfds, n_clients + 2, -1);
		if (pcount == -1)
			break ;
		for (int i = 0; i < n_clients + 2; ++i)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sfd)
					handle_signals(pfds, &shutdown_requested);
				else if (i == 0)
				{
					struct sockaddr_in	client;
					socklen_t			client_len = sizeof(client);
					int client_fd = accept(pfds[0].fd, (struct sockaddr *)&client, &client_len);
					if (client_fd == -1)
						continue ;
					if (n_clients >= MAX_CLIENTS)
					{
						send(client_fd, "ft_shield: Connection refused\r\n", 32, 0);
						close(client_fd);
						continue ;
					}
					pfds[2 + n_clients] = (struct pollfd){client_fd, POLLIN, 0};
					++n_clients;
				}
				else
					if (handle_client_input(pfds[i].fd) == -1)
						return ;
			}
		}
		if (shutdown_requested)
		{
			shutdown_server(pfds, &running, n_clients);
			n_clients = 0;
			break ;
		}
	}
}