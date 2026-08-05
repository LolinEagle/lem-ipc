#include "lemipc.h"

void	lock_sem(int semid)
{
	struct sembuf	sb;

	sb = (struct sembuf){0, -1, 0};
	semop(semid, &sb, 1);
}

void	unlock_sem(int semid)
{
	struct sembuf	sb;

	sb = (struct sembuf){0, 1, 0};
	semop(semid, &sb, 1);
}

int	init_ipcs(t_game *game)
{
	key_t	key;

	key = ftok(SHM_KEY_PATH, SHM_KEY_ID);
	if (key == -1)
	{
		perror("ftok");
		return (-1);
	}

	// Try to create shared memory exclusively
	game->shmid = shmget(key, sizeof(t_board), IPC_CREAT | IPC_EXCL | 0666);
	if (game->shmid != -1)
	{
		// First process: creator of resources
		game->is_creator = 1;
		game->semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
		game->msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

		if (game->semid == -1 || game->msgid == -1)
		{
			perror("semget/msgget");
			return (-1);
		}

		// Initialize mutex semaphore to 1
		semctl(game->semid, 0, SETVAL, 1);

		// Attach SHM and initialize empty board
		game->board = (t_board *)shmat(game->shmid, NULL, 0);
		if (game->board == (void *)-1)
		{
			perror("shmat");
			return (-1);
		}

		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++)
				game->board->grid[y][x] = 0;
		game->board->active_players = 0;
	}
	else
	{
		// Subsequent processes: connect to existing IPCs
		game->is_creator = 0;
		game->shmid = shmget(key, sizeof(t_board), 0666);
		game->semid = semget(key, 1, 0666);
		game->msgid = msgget(key, 0666);

		if (game->shmid == -1 || game->semid == -1 || game->msgid == -1)
		{
			perror("shmget/semget/msgget client");
			return (-1);
		}

		game->board = (t_board *)shmat(game->shmid, NULL, 0);
		if (game->board == (void *)-1)
		{
			perror("shmat");
			return (-1);
		}
	}
	return (0);
}

void	cleanup_ipcs(t_game *game)
{
	shmctl(game->shmid, IPC_RMID, NULL);
	semctl(game->semid, 0, IPC_RMID);
	msgctl(game->msgid, IPC_RMID, NULL);
	printf("[lemipc] Last player left. Cleaned up IPC resources.\n");
}
