#include "reversi.h"
#include "sleep.h"
#include <stdio.h>
#include <stdlib.h>

static void clear_screen(void) { printf("\x1b[1J\x1b[1;1H"); }

static void print_board(GameState *game) {
  printf("--- Reversi ---\n\n");
  printf("  0 1 2 3 4 5 6 7\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    printf("%d ", i);
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (game->board[i][j] == BLACK)
        printf("X ");
      else if (game->board[i][j] == WHITE)
        printf("O ");
      else
        printf(". ");
    }
    printf("\n");
  }
  printf("\n");
}

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
  game->consecutive_passes = 0;
}

Bool is_out_of_bounds(const GameState *game, Position pos) {
  if (pos.row < 0 || pos.row >= BOARD_SIZE || pos.col < 0 ||
      pos.col >= BOARD_SIZE) {
    return TRUE; // Out of bounds
  }
  // Additional logic to check if the move is valid according to Reversi rules
  // can be added here.
  return FALSE; // For now, just check if the cell is empty and within bounds
}

Bool is_not_empty(const GameState *game, Position pos) {
  if (game->board[pos.row][pos.col] != EMPTY) {
    return TRUE;
  }
  return FALSE;
}

Bool is_valid_move(const GameState *game, Position pos) {
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

static Bool can_place(const GameState *game, Position pos) {
  // try to place a piece on the board and check if it results in a valid move
  GameState temp = *game;
  if (game_place_piece(&temp, pos) == MOVE_SUCCESS) {
    return TRUE;
  }
  return FALSE;
}

PositionList game_get_valid_moves(const GameState *game) {
  PositionList list;
  list.count = 0;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      Position pos = {row, col};
      if (can_place(game, pos) == TRUE) {
        list.moves[list.count++] = pos;
      }
    }
  }

  return list;
}

Bool game_pass(GameState *game) {
  PositionList valid_moves = game_get_valid_moves(game);
  if (valid_moves.count == 0) {
    game->consecutive_passes++;
    game->current_player = get_opponent(game->current_player);
    return TRUE; // Pass successful
  }
  return FALSE; // Cannot pass, valid moves available
}

GameResultDetails game_get_result(const GameState *game) {
  GameResultDetails result;
  if (game->consecutive_passes < 2) {
    result.result = GAME_CONTINUE;
    result.black_count = 0;
    result.white_count = 0;
    return result;
  }

  int black_count = 0;
  int white_count = 0;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (game->board[row][col] == BLACK) {
        black_count++;
      } else if (game->board[row][col] == WHITE) {
        white_count++;
      }
    }
  }

  if (black_count > white_count) {
    result.result = BLACK_WIN;
  } else if (white_count > black_count) {
    result.result = WHITE_WIN;
  } else {
    result.result = DRAW;
  }
  result.black_count = black_count;
  result.white_count = white_count;
  return result;
}

Position select_random_move(const PositionList *list) {
  if (list->count == 0) {
    return (Position){-1, -1}; // No valid moves
  }
  int index = rand() % list->count;
  return list->moves[index];
}

static Position get_user_input(void) {
  Position pos;
  printf("Enter your move (row col): ");
  scanf("%d %d", &pos.row, &pos.col);
  return pos;
}

static void display_results(const GameState *game) {
  GameResultDetails result = game_get_result(game);
  if (result.result == BLACK_WIN) {
    printf("Black wins!\n");
    printf("Black: %d, White: %d\n", result.black_count, result.white_count);
  } else if (result.result == WHITE_WIN) {
    printf("White wins!\n");
    printf("Black: %d, White: %d\n", result.black_count, result.white_count);
  } else if (result.result == DRAW) {
    printf("It's a draw!\n");
    printf("Black: %d, White: %d\n", result.black_count, result.white_count);
  }
}

int play_reversi(void) {
  GameState game;
  init_game(&game);

  while (game_get_result(&game).result == GAME_CONTINUE) {
    PositionList valid_moves = game_get_valid_moves(&game);

    if (valid_moves.count == 0) {
      // auto pass if no valid moves are available
      game_pass(&game);
      continue;
    }

    clear_screen();
    print_board(&game);

    Position move;
    if (game.current_player == BLACK) {
      // player (black) makes a move
      // move = get_user_input();
      sleep_ms(500); // Simulate a delay for the player's move
      move = select_random_move(&valid_moves); // For testing, we can use random
                                               // move for black as well
    } else {
      // AI (white) makes a move
      move = select_random_move(&valid_moves);
    }

    game_place_piece(&game, move);
  }

  clear_screen();
  print_board(&game);
  display_results(&game);
  return 0;
}
