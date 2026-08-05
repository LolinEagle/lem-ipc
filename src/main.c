#include "lemipc.h"

t_game	*g_game = NULL;

void	handle_signal(int sig)
{
	(void)sig;
	if (g_game && g_game->board)
	{
		lock_sem(g_game->semid);
		g_game->board->grid[g_game->pos_y][g_game->pos_x] = 0;
		g_game->board->active_players--;

		if (g_game->board->active_players == 0)
			cleanup_ipcs(g_game);
		else
		{
			unlock_sem(g_game->semid);
			shmdt(g_game->board);
		}
	}
	printf("\n[lemipc] Player terminated cleanly.\n");
	exit(0);
}

int	main(int argc, char **argv)
{
	int		team_id;
	t_game	game;
	int		placed;
	int		x;
	int		y;

	if (argc != 2)
	{
		printf("Usage: %s <team_id>\n", argv[0]);
		return (1);
	}
	team_id = atoi(argv[1]);
	if (team_id <= 0)
	{
		printf("Error: Invalid team ID (must be > 0)\n");
		return (1);
	}

	g_game = &game;
	game.team_id = team_id;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	if (init_ipcs(&game) == -1)
		return (1);

	lock_sem(game.semid);

	// Spawn player on a random empty tile
	srand(time(NULL) ^ getpid());
	placed = 0;
	for (int attempts = 0; attempts < 1000; attempts++)
	{
		x = rand() % BOARD_WIDTH;
		y = rand() % BOARD_HEIGHT;
		if (game.board->grid[y][x] == 0)
		{
			game.pos_x = x;
			game.pos_y = y;
			game.board->grid[y][x] = team_id;
			game.board->active_players++;
			placed = 1;
			break ;
		}
	}

	if (!placed)
	{
		printf("Error: Map is full!\n");
		unlock_sem(game.semid);
		shmdt(game.board);
		return (1);
	}

	unlock_sem(game.semid);

	// Main game loop
	while (1)
	{
		lock_sem(game.semid);

		// 1. Death check
		if (check_death(&game))
		{
			printf("Player from team %d died at (%d, %d)!\n", game.team_id,
				game.pos_x, game.pos_y);
			game.board->grid[game.pos_y][game.pos_x] = 0;
			game.board->active_players--;

			if (game.board->active_players == 0)
				cleanup_ipcs(&game);
			else
			{
				unlock_sem(game.semid);
				shmdt(game.board);
			}
			exit(0);
		}

		// 2. Execute movement AI
		move_player(&game);

		// 3. Render board
		display_board(game.board);
		unlock_sem(game.semid);
		usleep(300000);// Tick speed (300 ms)
	}

	return (0);
}
