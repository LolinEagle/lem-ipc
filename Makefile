NAME		=	lemipc
SRC			=	main.c ipc.c game.c
CC			=	gcc
RM			=	rm -rf
FLAGS		=	-Wall -Werror -Wextra
INC			=	$(addprefix -I,ft_printf inc)
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/
OBJ_DIRS	=	$(OBJ_PATH)
OBJ			=	$(addprefix $(OBJ_PATH),$(SRC:.c=.o))
ERROR		:=	EMPTY_LINE_FUNCTION \
				GLOBAL_VAR_DETECTED \
				TOO_MANY_LINES \
				WRONG_SCOPE_COMMENT
IGNORE		:=	$(addprefix -e,$(ERROR))

all:$(NAME)

clean:
	$(MAKE) -sC ft_printf clean
	$(RM) $(OBJ_PATH) .vscode

fclean:clean
	$(MAKE) -sC ft_printf fclean
	$(RM) $(NAME)

re:fclean
	$(MAKE) all

run:re
	./$(NAME) 1

norm:
	@norminette | grep -v $(IGNORE) || true

$(OBJ_PATH)%.o:$(SRC_PATH)%.c | $(OBJ_DIRS)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)

$(NAME):$(OBJ)
	$(MAKE) -sC ft_printf
	$(CC) $(FLAGS) $(INC) $(OBJ) ft_printf/libftprintf.a -o $(NAME)

.PHONY:all clean fclean re run norm
