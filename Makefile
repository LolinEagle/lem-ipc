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
IGNORE		:=	en_US|DECL_ASSIGN_LINE|EMPTY_LINE_FUNCTION|FORBIDDEN_CS
IGNORE		:=	$(IGNORE)|GLOBAL_VAR_DETECTED|INVALID_HEADER|TOO_MANY_LINES
IGNORE		:=	$(IGNORE)|WRONG_SCOPE_COMMENT

all:$(NAME)

clean:
	$(RM) $(OBJ_PATH) .vscode

fclean:clean
	$(RM) $(NAME)

re:fclean
	$(MAKE) all

norminette:
	@norminette | grep -vE "$(IGNORE)"

$(OBJ_PATH)%.o:$(SRC_PATH)%.c | $(OBJ_DIRS)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)

$(NAME):$(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)

.PHONY:all clean fclean re norminette
