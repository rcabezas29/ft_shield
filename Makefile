# **************************************************************************** #
#                                   PROGRAM                                    #
# **************************************************************************** #

NAME = $(BUILD_DIR)/ft_shield

# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

MAKE = make

CP = cp
MV = mv
MKDIR = mkdir -p
RM = rm -rf

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow -g3

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

BUILD_DIR := build
INC_DIR := includes

SRCS := srcs/commands.c srcs/daemon.c srcs/main.c srcs/password.c srcs/server.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CFLAGS += -I./$(INC_DIR)
LDFLAGS += -I/opt/ssl/include/ -L/opt/ssl/lib/ -lcrypto

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS) $(LDLIBS)
	$(CC) $(CFLAGS) -D PASSWORD=password -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BUILD_DIR)

re:: clean
re:: all

-include $(DEPS)

.PHONY: all sanitize clean fclean re
