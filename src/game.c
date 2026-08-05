#include "lemipc.h"

void	display_board(const t_board *board)
{
	printf("\33[H\33[J");// Clear screen
	printf("=== LEMIPC BOARD (%d active players) ===\n", board->active_players);
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (board->grid[y][x] == 0)
				printf(". ");
			else
				printf("%d ", board->grid[y][x]);
		}
		printf("\n");
	}
	printf("========================================\n");
}

// Returns 1 if player is surrounded by 2+ players from the SAME opposing team
int	check_death(t_game *game)
{
	int	team_counts[100] = {0};
	int	nx;
	int	ny;
	int	enemy_team;

	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (dx == 0 && dy == 0)
				continue ;

			nx = game->pos_x + dx;
			ny = game->pos_y + dy;

			if (nx >= 0 && nx < BOARD_WIDTH && ny >= 0 && ny < BOARD_HEIGHT)
			{
				enemy_team = game->board->grid[ny][nx];
				if (enemy_team > 0 && enemy_team != game->team_id)
				{
					if (enemy_team < 100)
						team_counts[enemy_team]++;
				}
			}
		}
	}

	for (int t = 0; t < 100; t++)
	{
		if (team_counts[t] >= 2)
			return (1);
	}
	return (0);
}

static void	find_closest_enemy(t_game *game, int *target_x, int *target_y)
{
	int	min_dist = 999999;
	int	cell_team;
	int	dist;

	*target_x = -1;
	*target_y = -1;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			cell_team = game->board->grid[y][x];
			if (cell_team > 0 && cell_team != game->team_id)
			{
				dist = abs(game->pos_x - x) + abs(game->pos_y - y);
				if (dist < min_dist)
				{
					min_dist = dist;
					*target_x = x;
					*target_y = y;
				}
			}
		}
	}
}

void	move_player(t_game *game)
{
	t_msg	msg;
	int		tx = -1, ty = -1;
	int		next_x = game->pos_x;
	int		next_y = game->pos_y;
	int		dir;

	// Check MSGQ for team strategy message
	if (msgrcv(game->msgid, &msg, sizeof(t_msg) - sizeof(long), game->team_id,
			IPC_NOWAIT) != -1)
	{
		tx = msg.target_x;
		ty = msg.target_y;
	}
	else
	{
		// Find closest enemy and broadcast to teammates
		find_closest_enemy(game, &tx, &ty);
		if (tx != -1)
		{
			msg.mtype = game->team_id;
			msg.target_x = tx;
			msg.target_y = ty;
			msgsnd(game->msgid, &msg, sizeof(t_msg) - sizeof(long), IPC_NOWAIT);
		}
	}

	if (tx != -1)
	{
		// Move 1 tile toward target (up, down, left, right)
		if (next_x < tx)
			next_x++;
		else if (next_x > tx)
			next_x--;
		else if (next_y < ty)
			next_y++;
		else if (next_y > ty)
			next_y--;
	}
	else
	{
		// Random movement if no enemy on board
		dir = rand() % 4;
		if (dir == 0 && next_y > 0)
			next_y--;
		else if (dir == 1 && next_y < BOARD_HEIGHT - 1)
			next_y++;
		else if (dir == 2 && next_x > 0)
			next_x--;
		else if (dir == 3 && next_x < BOARD_WIDTH - 1)
			next_x++;
	}

	// Perform movement if destination cell is empty
	if (game->board->grid[next_y][next_x] == 0)
	{
		game->board->grid[game->pos_y][game->pos_x] = 0;
		game->pos_x = next_x;
		game->pos_y = next_y;
		game->board->grid[game->pos_y][game->pos_x] = game->team_id;
	}
}
