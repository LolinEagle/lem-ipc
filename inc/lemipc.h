#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <errno.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10
#define SHM_KEY_PATH "."
#define SHM_KEY_ID 'L'

// Shared memory structure
typedef struct s_board
{
	int	grid[BOARD_HEIGHT][BOARD_WIDTH];
	int	active_players;
}	t_board;

// Message Queue structure
typedef struct s_msg
{
	long	mtype;// Target team ID
	int		target_x;
	int		target_y;
}	t_msg;

// Player state structure
typedef struct s_game
{
	int		shmid;
	int		semid;
	int		msgid;
	t_board	*board;
	int		team_id;
	int		pos_x;
	int		pos_y;
	int		is_creator;
}	t_game;

// Global pointer for signal cleanup
extern t_game	*g_game;

// IPC functions
int		init_ipcs(t_game *game);
void	cleanup_ipcs(t_game *game);
void	lock_sem(int semid);
void	unlock_sem(int semid);

// Game functions
void	display_board(const t_board *board);
int		check_death(t_game *game);
void	move_player(t_game *game);
