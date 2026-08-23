#ifndef REVERSI_H
#define REVERSI_H
#define BOARD_SIZE 8
#define FALSE 0
#define TRUE 1

typedef enum { EMPTY, BLACK, WHITE } CellState;
typedef enum {
  MOVE_SUCCESS,
  MOVE_INVALID,
  MOVE_NO_FLIP,
  MOVE_OUT_OF_BOUNDS,
  MOVE_CELL_NOT_EMPTY
} MoveResult;
typedef struct {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  CellState current_player;
} GameState;

typedef struct {
  int row;
  int col;
} Position;

typedef struct {
  int row_delta;
  int col_delta;
} Direction;

void clear_screen(void);
void init_game(GameState *game);
int is_out_of_bounds(const GameState *game, Position pos);
int is_not_empty(const GameState *game, Position pos);
int is_valid_move(const GameState *game, Position pos);
MoveResult game_place_piece(GameState *game, Position pos);
int play_reversi(void);

#endif // REVERSI_H
