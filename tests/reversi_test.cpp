#include <gtest/gtest.h>

extern "C" {
#include "reversi.h"
}

TEST(CoreReversiTest, CanPlaceOnEmptyCell) {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  GameState game;
  init_game(&game);
  EXPECT_EQ(BLACK, game.current_player);
  EXPECT_EQ(WHITE, game.board[3][3]);
  EXPECT_EQ(WHITE, game.board[4][4]);
  EXPECT_EQ(BLACK, game.board[3][4]);
  EXPECT_EQ(BLACK, game.board[4][3]);
  EXPECT_EQ(EMPTY, game.board[0][0]);
  EXPECT_EQ(EMPTY, game.board[7][7]);
}

TEST(PositionTest, IsValidPosition) {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  GameState game;
  init_game(&game);
  EXPECT_TRUE(is_valid_move(&game, {0, 0}));
  EXPECT_TRUE(is_valid_move(&game, {7, 7}));
  EXPECT_FALSE(is_valid_move(&game, {-1, 0}));
  EXPECT_FALSE(is_valid_move(&game, {0, 8}));
}

TEST(GameTest, PlaceDiskAndSwitchPlayer) {
  GameState game;
  init_game(&game);

  /*
  --- Reversi ---
    0 1 2 3 4 5 6 7
  0 . . . . . . . .
  1 . . . . . . . .
  2 . . . X . . . .
  3 . . . O X . . .
  4 . . . X O . . .
  5 . . . . . . . .
  6 . . . . . . . .
  7 . . . . . . . .
  */
  // Position pos = {2, 3};
  // MoveResult result = game_place_piece(&game, pos);

  EXPECT_EQ(MOVE_SUCCESS, game_place_piece(&game, {2, 3}));
  EXPECT_EQ(game.board[2][3], BLACK);
  EXPECT_EQ(game.board[3][3], BLACK);
  EXPECT_EQ(game.current_player, WHITE);

  init_game(&game);
  EXPECT_EQ(MOVE_CELL_NOT_EMPTY, game_place_piece(&game, {3, 4}));
  EXPECT_EQ(game.board[3][4], BLACK);
  EXPECT_EQ(game.current_player, BLACK);

  EXPECT_EQ(MOVE_NO_FLIP, game_place_piece(&game, {0, 0}));
  EXPECT_EQ(game.board[0][0], EMPTY);
  EXPECT_EQ(game.board[3][3], WHITE);
  EXPECT_EQ(game.current_player, BLACK);

  EXPECT_EQ(MOVE_OUT_OF_BOUNDS, game_place_piece(&game, {-1, 0}));
  EXPECT_EQ(game.board[0][0], EMPTY);
  EXPECT_EQ(game.board[3][3], WHITE);
  EXPECT_EQ(game.current_player, BLACK);

  // EXPECT_TRUE(result);
}
