#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

void * checkRow(void * param);
void * checkColumn(void * param);
void * checkDiagonal(void * param);

char winner[1];
char game_board[3][3];

int main(int argc, char *argv[])
{
  // For the pipe.
  char write_msg[BUFFER_SIZE] = "Winner of the game.";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int fd[2];
  // For the pthreads.
  pthread_t thread1, thread2, thread3; /* thread identifiers */
  char boardchars[9]; /* char array to pass tictactoe values to. */
  int c,r; /* column and row counter. */

  scanf("%s", boardchars);
  // Copu passed argument int char array.
  //strcpy(boardchars,argv[1]);

  /* Assign chars from boardchars to game board.*/
  int i = 0;
  for(r=1; r<=3; r++) {
    for(c=1; c<=3; c++) {
        game_board[r][c] = boardchars[i];
        i++;
    }
  }

  /* Printing the gameboard for visualization*/
  for (c = 1; c <= 3; c++) {
       for (r = 1; r <= 3; r++) {
           printf("%c ", game_board[c][r]);
       }
       printf("\n");
   }

  /* Now create the thread passing it data as a parameter */
  pthread_create(&thread1, NULL, checkRow, NULL);
  pthread_create(&thread2, NULL, checkColumn, NULL);
  pthread_create(&thread3, NULL, checkDiagonal, NULL);

  /* Joining threads. */
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  strcpy(write_msg, winner);

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		close(fd[READ_END]);
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		close(fd[WRITE_END]);
	}
	else { /* child process */
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("The winner of this tictactoe game is:%s\n",read_msg);
		close(fd[READ_END]);
	}

	return 0;
}

// Check each row.
void * checkRow(void * param)
{
    printf("Checking Rows\n");
    int i, j;
    int x_count = 0;
    int o_count = 0;

    for (i = 1; i <= 3; ++i) {
        for (j = 1; j <= 3; ++j) {
              char val = game_board[i][j];
              if (val == 'x') {
                  x_count++;
              }
              else if (val == 'o'){
                  o_count++;
              }
        }

        /* Check for winner. */
        if (x_count == 3) {
          strcpy(winner, "x");
        }
        else if (o_count == 3) {
          strcpy(winner, "o");
        }
        else{
          x_count = 0;
          o_count = 0;
        }
    }

	pthread_exit(0);
}

// Check each column.
void * checkColumn (void *param)
{
  printf("Check Columns\n");
  int i, j;
  int x_count = 0;
  int o_count = 0;

  for (i = 1; i <= 3; ++i) {
      for (j = 1; j <= 3; ++j) {
          char val = game_board[j][i];
          if (val == 'x') {
              x_count++;
          }
          else if (val == 'o'){
              o_count++;
          }
      }

      /* Check for winner. */
      if (x_count == 3) {
        strcpy(winner, "x");
      }
      else if (o_count == 3) {
        strcpy(winner, "o");
      }
      else{
        x_count = 0;
        o_count = 0;
      }
  }

	pthread_exit(0);
}

// Check both Diagonals.
void * checkDiagonal (void *param)
{
  printf("Checking Diagonal.\n");
  int i, j;
  int x_count = 0;
  int o_count = 0;

  for (i = 1; i <= 3; ++i) {
      char val = game_board[i][i];
      if (val == 'x') {
          x_count++;
      }
      else if (val == 'o'){
          o_count++;
      }
  }

  /* Check for winner. */
  if (x_count == 3) {
    strcpy(winner, "x");
  }
  else if (o_count == 3) {
    strcpy(winner, "o");
  }
  else{
    x_count = 0;
    o_count = 0;
  }

  printf("Checking other Diagonal.\n");
  for (i = 1; i <= 3; ++i) {
      char val = game_board[i][4-i];
      if (val == 'x') {
          x_count++;
      }
      else if (val == 'o'){
          o_count++;
      }
  }

  /* Check for winner. */
  if (x_count == 3) {
    strcpy(winner, "x");
  }
  else if (o_count == 3) {
    strcpy(winner, "o");
  }
  else{
    x_count = 0;
    o_count = 0;
  }

  pthread_exit(0);
}
