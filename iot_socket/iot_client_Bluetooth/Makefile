CC = cc
NAME = iot_client_bluetooth
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lbluetooth -pthread
INCS = incs
SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

.c.o :
	$(CC) -c $(CFLAGS) -o $@ $^ -I $(INCS)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re :
	make fclean
	make all

only :
	make re
	make clean

.PHONY : all clean fclean re .c.o