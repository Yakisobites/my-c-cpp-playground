#include <gtest/gtest.h>

extern "C" {
#include "reversi.h"
}

TEST(CoreReversiTest, CanPlaceOnEmptyCell) {
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
  GameState game;
  init_game(&game);
  EXPECT_TRUE(is_valid_move(&game, {2, 3}));
  EXPECT_TRUE(is_valid_move(&game, {3, 2}));
  EXPECT_FALSE(is_valid_move(&game, {0, 0}));
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
}

TEST(GameTest, GetValidMovesInitialState) {
  GameState game;
  init_game(&game);

  PositionList list = game_get_valid_moves(&game);
  EXPECT_EQ(list.count, 4);
}

TEST(GameTest, ConsecutivePassesEndGame) {
  GameState game;
  init_game(&game);

  // create a board where no valid moves are available for either player
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      game.board[r][c] = (c < 4) ? BLACK : WHITE;
    }
  }

  // it is expected that the first pass will succeed because there are no valid
  // moves for the current player. The consecutive_passes counter should
  // increment to 1, and the game should continue.
  EXPECT_TRUE(game_pass(&game));
  EXPECT_EQ(game.consecutive_passes, 1);
  EXPECT_EQ(game_get_result(&game).result, GAME_CONTINUE);

  // it is expected that the second pass will also succeed because there are no
  // valid moves for the opponent.
  EXPECT_TRUE(game_pass(&game));
  EXPECT_EQ(game.consecutive_passes, 2);

  // it is expected that the game ends after two consecutive passes (black 32
  // discs, white 32 discs, resulting in a draw)
  EXPECT_EQ(game_get_result(&game).result, DRAW);
}

TEST(GameTest, CannotPassWhenValidMoveExists) {
  GameState game;
  init_game(&game);

  // Failed to pass when valid moves exist
  EXPECT_EQ(game_pass(&game), FALSE);
}
