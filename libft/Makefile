# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <marvin@codam.nl>                   +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings       #+#    #+#                 #
#    Updated: 2019/12/20 09:17:37 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SOURCES = $(FILES:%=ft_%.c)
BSOURCES = $(BFILES:%=ft_%.c)
OBJECTS = $(FILES:%=ft_%.o)
BOBJECTS = $(BFILES:%=ft_%.o)
FLAGS = -Wall -Wextra -Werror
HEADERS = libft.h
FILES = tolower toupper isprint isascii isalnum isdigit isalpha atoi \
		strncmp strnstr strrchr strchr strlcat strlcpy strlen memcmp \
		memchr memmove memccpy memcpy bzero memset calloc strdup \
		substr strjoin strtrim split itoa strmapi putchar_fd \
		putstr_fd putendl_fd putnbr_fd

BFILES = lstnew lstadd_front lstsize lstlast lstadd_back lstdelone \
		 lstclear lstiter lstmap

all: $(NAME)

$(NAME):
	gcc $(FLAGS) -I $(HEADERS) -c $(SOURCES)
	ar rc $(NAME) $(OBJECTS)
	ranlib $(NAME)

clean:
	/bin/rm -f $(OBJECTS) $(BOBJECTS)
	/bin/rm -f bonus

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

bonus:
	gcc $(FLAGS) -I $(HEADERS) -c $(BSOURCES) -c $(SOURCES)
	ar rc $(NAME) $(OBJECTS) $(BOBJECTS)
	touch bonus

.PHONY: all clean fclean re
