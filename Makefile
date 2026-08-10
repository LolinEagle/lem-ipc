NAME		=	lemipc
SRC			=	main.c ipc.c game.c
CC			=	gcc
RM			=	rm -rf
FLAGS		=	-Wall -Werror -Wextra
INC			=	-Iinc
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/
OBJ_DIRS	=	$(OBJ_PATH)
OBJ			=	$(addprefix $(OBJ_PATH),$(SRC:.c=.o))
IGNORE		:=	GLOBAL_VAR_DETECTED \
				TOO_MANY_LINES \
				WRONG_SCOPE_COMMENT
IGNORE_FLAGS := $(addprefix -e ,$(IGNORE))

all:$(NAME)

clean:
	$(RM) $(OBJ_PATH) .vscode

fclean:clean
	$(RM) $(NAME)

re:fclean
	$(MAKE) all

run:re
	./$(NAME) 1

norm:
	@norminette | grep -v $(IGNORE_FLAGS) || true

$(OBJ_PATH)%.o:$(SRC_PATH)%.c | $(OBJ_DIRS)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)

$(NAME):$(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)

.PHONY:all clean fclean re run norm
