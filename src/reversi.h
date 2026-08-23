#ifndef REVERSI_H
#define REVERSI_H
#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2

void clear_screen(void);
void init_board(int board[BOARD_SIZE][BOARD_SIZE]);
void print_board(int board[BOARD_SIZE][BOARD_SIZE]);
int can_place(int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int player);
void place_and_flip(int board[BOARD_SIZE][BOARD_SIZE], int r, int c,
                    int player);
int has_valid_move(int board[BOARD_SIZE][BOARD_SIZE], int player);
void count_pieces(int board[BOARD_SIZE][BOARD_SIZE], int *black_count,
                  int *white_count);
int play_reversi(void);

#endif // REVERSI_H
