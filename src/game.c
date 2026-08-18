/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <frrusso@learner.42.tech>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:14:00 by frrusso           #+#    #+#             */
/*   Updated: 2026/08/07 16:14:03 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	display_board(const t_board *board)
{
	t_vec2	pos;

	ft_printf("\33[H\33[J");// Clear screen
	ft_printf("--- LEM-IPC BOARD (%d active players) ---\n\n",
		board->active_players);
	pos.y = -1;
	while (++pos.y < BOARD_HEIGHT)
	{
		ft_printf("          ");
		pos.x = -1;
		while (++pos.x < BOARD_WIDTH)
		{
			if (board->grid[pos.y][pos.x] == 0)
				ft_printf(". ");
			else
				ft_printf("%d ", board->grid[pos.y][pos.x]);
		}
		ft_printf("\n");
	}
	ft_printf("\n----------------------------------------\n");
}

// Returns 1 if player is surrounded by 2+ players from the SAME opposing team
int	check_death(t_game *game)
{
	int		i;
	int		team_counts[10];
	t_vec2	d;
	t_vec2	n;
	int		enemy_team;

	i = -1;
	while (++i < 10)
		team_counts[i] = 0;
	d.y = -2;
	while (++d.y <= 1)
	{
		d.x = -2;
		while (++d.x <= 1)
		{
			if (d.x == 0 && d.y == 0)
				continue ;
			n.x = game->pos_x + d.x;
			n.y = game->pos_y + d.y;
			if (n.x >= 0 && n.x < BOARD_WIDTH && n.y >= 0 && n.y < BOARD_HEIGHT)
			{
				enemy_team = game->board->grid[n.y][n.x];
				if (enemy_team > 0 && enemy_team != game->team_id)
				{
					if (enemy_team < 10)
						team_counts[enemy_team]++;
				}
			}
		}
	}
	i = -1;
	while (++i < 10)
	{
		if (team_counts[i] >= 2)
			return (1);
	}
	return (0);
}

static void	find_closest_enemy(t_game *game, int *target_x, int *target_y)
{
	int		min_dist;
	t_vec2	pos;
	int		cell_team;
	int		dist;

	*target_x = -1;
	*target_y = -1;
	min_dist = 999999;
	pos.y = -1;
	while (++pos.y < BOARD_HEIGHT)
	{
		pos.x = -1;
		while (++pos.x < BOARD_WIDTH)
		{
			cell_team = game->board->grid[pos.y][pos.x];
			if (cell_team > 0 && cell_team != game->team_id)
			{
				dist = abs(game->pos_x - pos.x) + abs(game->pos_y - pos.y);
				if (dist < min_dist)
				{
					min_dist = dist;
					*target_x = pos.x;
					*target_y = pos.y;
				}
			}
		}
	}
}

void	random_movement(t_vec2 *next)
{
	int		dir;

	dir = rand() % 4;
	if (dir == 0 && next->y > 0)
		next->y--;
	else if (dir == 1 && next->y < BOARD_HEIGHT - 1)
		next->y++;
	else if (dir == 2 && next->x > 0)
		next->x--;
	else if (dir == 3 && next->x < BOARD_WIDTH - 1)
		next->x++;
}

void	move_player(t_game *game)
{
	t_msg	msg;
	t_vec2	t;
	t_vec2	next;

	next.x = game->pos_x;
	next.y = game->pos_y;
	t.x = -1;
	t.y = -1;

	// Check MSGQ for team strategy message
	if (msgrcv(game->msgid, &msg, sizeof(t_msg) - sizeof(long), game->team_id,
			IPC_NOWAIT) != -1)
	{
		t.x = msg.target_x;
		t.y = msg.target_y;
	}
	else
	{
		// Find closest enemy and broadcast to teammates
		find_closest_enemy(game, &t.x, &t.y);
		if (t.x != -1)
		{
			msg.mtype = game->team_id;
			msg.target_x = t.x;
			msg.target_y = t.y;
			msgsnd(game->msgid, &msg, sizeof(t_msg) - sizeof(long), IPC_NOWAIT);
		}
	}
	if (t.x != -1)
	{
		// Move 1 tile toward target else randomly (up, down, left, right)
		if (rand() % 10 == 0)
			random_movement(&next);// 10% chance of moveing randomly
		else if (next.x < t.x && game->board->grid[next.y][next.x + 1] == 0)
			next.x++;
		else if (next.x > t.x && game->board->grid[next.y][next.x - 1] == 0)
			next.x--;
		else if (next.y < t.y && game->board->grid[next.y + 1][next.x] == 0)
			next.y++;
		else if (next.y > t.y && game->board->grid[next.y - 1][next.x] == 0)
			next.y--;
		else
			random_movement(&next);
	}
	else
	{
		// Random movement if no enemy on board
		random_movement(&next);
	}

	// Perform movement if destination cell is empty
	if (game->board->grid[next.y][next.x] == 0)
	{
		game->board->grid[game->pos_y][game->pos_x] = 0;
		game->pos_x = next.x;
		game->pos_y = next.y;
		game->board->grid[game->pos_y][game->pos_x] = game->team_id;
	}
}
