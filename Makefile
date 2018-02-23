##
## EPITECH PROJECT, 2018
## nmobjdump
## File description:
## objdump
##

SRC	=	src/main.c	\
		src/begin.c	\
		src/print_data.c\
		src/print_nbr.c	\
		src/version_32.c\
		src/version_64.c

CFLAGS	=	-W -Wall -Werror -Wextra -Iincludes

OBJ	=	$(SRC:.c=.o)

NAME	=	objdump

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all