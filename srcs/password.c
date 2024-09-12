#include <ft_shield.h>

char	*generate_cypher_password(void)
{
	char	*password;
	int		len = strlen(PASSWORD);

	password = malloc(len + 1);
	if (!password)
		exit(EXIT_FAILURE);
	password[len] = '\0';
	for (int i = 0; i < len; ++i)
		password[i] = PASSWORD[i] ^ 42;
	return password;
}

char	*decode_password(char *password)
{
	int		len = strlen(password);
	for (int i = 0; i < len; ++i)
		password[i] ^= 42;
	return password;
}
