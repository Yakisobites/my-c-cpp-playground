#ifndef REVERSI_H
#define REVERSI_H
#define BOARD_SIZE 8
#define TOTAL_CELLS (BOARD_SIZE * BOARD_SIZE)

typedef enum { FALSE, TRUE } Bool;
typedef enum { EMPTY, BLACK, WHITE } CellState;
typedef enum {
  MOVE_SUCCESS,
  MOVE_INVALID,
  MOVE_NO_FLIP,
  MOVE_OUT_OF_BOUNDS,
  MOVE_CELL_NOT_EMPTY
} MoveResult;
typedef enum { GAME_CONTINUE, BLACK_WIN, WHITE_WIN, DRAW } GameResult;
typedef struct {
  GameResult result;
  int black_count;
  int white_count;
} GameResultDetails;
typedef struct {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  CellState current_player;
  int consecutive_passes;
} GameState;

typedef struct {
  int row;
  int col;
} Position;

typedef struct {
  int row_delta;
  int col_delta;
} Direction;
typedef struct {
  Position moves[TOTAL_CELLS];
  int count;
} PositionList;

void clear_screen(void);
void init_game(GameState *game);
Bool is_out_of_bounds(const GameState *game, Position pos);
Bool is_not_empty(const GameState *game, Position pos);
Bool is_valid_move(const GameState *game, Position pos);
MoveResult game_place_piece(GameState *game, Position pos);
PositionList game_get_valid_moves(const GameState *game);
Bool game_pass(GameState *game);
GameResultDetails game_get_result(const GameState *game);
Position select_random_move(const PositionList *list);
int play_reversi(void);

#endif // REVERSI_H
