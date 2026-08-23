#include "reversi.h"
#include <stdio.h>

void clear_screen(void) { printf("\x1b[1J\x1b[1;1H"); }

static CellState get_opponent(CellState player) {
  return (player == BLACK) ? WHITE : BLACK;
}

void init_game(GameState *game) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      game->board[i][j] = EMPTY;
    }
  }
  game->board[3][3] = WHITE;
  game->board[4][4] = WHITE;
  game->board[3][4] = BLACK;
  game->board[4][3] = BLACK;
  game->current_player = BLACK;
}

int is_out_of_bounds(const GameState *game, Position pos) {
  if (pos.row < 0 || pos.row >= BOARD_SIZE || pos.col < 0 ||
      pos.col >= BOARD_SIZE) {
    return TRUE; // Out of bounds
  }
  // Additional logic to check if the move is valid according to Reversi rules
  // can be added here.
  return FALSE; // For now, just check if the cell is empty and within bounds
}

int is_not_empty(const GameState *game, Position pos) {
  if (game->board[pos.row][pos.col] != EMPTY) {
    return TRUE;
  }
  return FALSE;
}

int is_valid_move(const GameState *game, Position pos) {
  if (is_out_of_bounds(game, pos)) {
    return FALSE;
  }
  if (is_not_empty(game, pos)) {
    return FALSE;
  }
  // Additional logic to check if the move is valid according to Reversi rules
  // can be added here.
  return TRUE; // For now, just check if the cell is empty and within bounds
}

static int flip_direction(GameState *game, Position pos, Direction dir) {
  CellState player = game->current_player;
  CellState opponent = get_opponent(player);

  Position cur = {pos.row + dir.row_delta, pos.col + dir.col_delta};
  int count = 0;

  while (!is_out_of_bounds(game, cur)) {
    if (game->board[cur.row][cur.col] == opponent) {
      count++;
    } else if (game->board[cur.row][cur.col] == player) {
      if (count > 0) {
        Position flip = {pos.row + dir.row_delta, pos.col + dir.col_delta};
        while (flip.row != cur.row || flip.col != cur.col) {
          game->board[flip.row][flip.col] = player;
          flip.row += dir.row_delta;
          flip.col += dir.col_delta;
        }
      }
      return count;
    } else {
      break;
    }
    cur.row += dir.row_delta;
    cur.col += dir.col_delta;
  }
  return 0;
}

MoveResult game_place_piece(GameState *game, Position pos) {
  if (is_out_of_bounds(game, pos)) {
    return MOVE_OUT_OF_BOUNDS;
  }

  if (is_not_empty(game, pos)) {
    return MOVE_CELL_NOT_EMPTY;
  }

  Direction directions[] = {{1, 0},  {-1, -1}, {0, -1}, {1, -1},
                            {-1, 0}, {-1, 1},  {0, 1},  {1, 1}};

  int total_flipped = 0;
  for (int i = 0; i < 8; i++) {
    total_flipped += flip_direction(game, pos, directions[i]);
  }

  if (total_flipped == 0) {
    return MOVE_NO_FLIP;
  }

  game->board[pos.row][pos.col] = game->current_player;
  game->current_player = get_opponent(game->current_player);

  return MOVE_SUCCESS;
}

int play_reversi(void) { return 0; }
