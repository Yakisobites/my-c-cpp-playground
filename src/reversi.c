#include "reversi.h"
#include <stdio.h>

void clear_screen(void) { printf("\x1b[1J\x1b[1;1H"); }

void init_board(int board[BOARD_SIZE][BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = EMPTY;
    }
  }
  board[3][3] = WHITE;
  board[4][4] = WHITE;
  board[3][4] = BLACK;
  board[4][3] = BLACK;
}

void print_board(int board[BOARD_SIZE][BOARD_SIZE]) {
  printf("--- Reversi ---\n\n");
  printf("  0 1 2 3 4 5 6 7\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    printf("%d ", i);
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == BLACK)
        printf("X ");
      else if (board[i][j] == WHITE)
        printf("O ");
      else
        printf(". ");
    }
    printf("\n");
  }
  printf("\n");
}

// 8方向（上、下、左、右、左上、右上、左下、右下）への移動量
const int DX[8] = {0, 0, -1, 1, -1, 1, -1, 1};
const int DY[8] = {-1, 1, 0, 0, -1, -1, 1, 1};

// 指定したマス(r, c)に player が石を置けるか判定（置ければ1、置けなければ0）
int can_place(int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int player) {
  if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE)
    return 0;
  if (board[r][c] != EMPTY)
    return 0;

  int opponent = (player == BLACK) ? WHITE : BLACK;

  for (int d = 0; d < 8; d++) {
    int nr = r + DY[d];
    int nc = c + DX[d];
    int count = 0;

    // 相手の石が続く限り進む
    while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
           board[nr][nc] == opponent) {
      nr += DY[d];
      nc += DX[d];
      count++;
    }

    // 相手の石が1個以上あり、その先に自分の石があれば設置可能
    if (count > 0 && nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
        board[nr][nc] == player) {
      return 1;
    }
  }
  return 0;
}

// 石を配置し、8方向の相手の石を裏返す
void place_and_flip(int board[BOARD_SIZE][BOARD_SIZE], int r, int c,
                    int player) {
  int opponent = (player == BLACK) ? WHITE : BLACK;
  board[r][c] = player;

  for (int d = 0; d < 8; d++) {
    int nr = r + DY[d];
    int nc = c + DX[d];
    int count = 0;

    while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
           board[nr][nc] == opponent) {
      nr += DY[d];
      nc += DX[d];
      count++;
    }

    if (count > 0 && nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
        board[nr][nc] == player) {
      int fr = r + DY[d];
      int fc = c + DX[d];
      // 自分の石にたどり着くまで裏返していく
      while (board[fr][fc] == opponent) {
        board[fr][fc] = player;
        fr += DY[d];
        fc += DX[d];
      }
    }
  }
}

// プレイヤーが置ける場所が1マスでも存在するか判定
int has_valid_move(int board[BOARD_SIZE][BOARD_SIZE], int player) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (can_place(board, r, c, player)) {
        return 1;
      }
    }
  }
  return 0;
}

// 盤面上の黒石・白石の数をカウント
void count_pieces(int board[BOARD_SIZE][BOARD_SIZE], int *black_count,
                  int *white_count) {
  *black_count = 0;
  *white_count = 0;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c] == BLACK)
        (*black_count)++;
      else if (board[r][c] == WHITE)
        (*white_count)++;
    }
  }
}

int play_reversi(void) {
  int board[BOARD_SIZE][BOARD_SIZE];
  int current_player = BLACK;
  int pass_count = 0;

  init_board(board);

  while (pass_count < 2) {
    clear_screen();
    print_board(board);

    // 置ける場所がない場合はパス処理
    if (!has_valid_move(board, current_player)) {
      printf("Player %s has no valid moves. Pass!\n",
             (current_player == BLACK) ? "BLACK (X)" : "WHITE (O)");
      printf("Press Enter to continue...");
      while (getchar() != '\n')
        ;
      getchar();

      pass_count++;
      current_player = (current_player == BLACK) ? WHITE : BLACK;
      continue;
    }

    // 1手でも指せたらパスのカウントをリセット
    pass_count = 0;

    printf("Player: %s\n",
           (current_player == BLACK) ? "BLACK (X)" : "WHITE (O)");
    printf("Enter row and column (e.g. 2 3): ");

    int r, c;
    if (scanf("%d %d", &r, &c) != 2) {
      while (getchar() != '\n')
        ;
      continue;
    }

    if (!can_place(board, r, c, current_player)) {
      printf("Invalid move! Press Enter to try again...");
      while (getchar() != '\n')
        ;
      getchar();
      continue;
    }

    place_and_flip(board, r, c, current_player);
    current_player = (current_player == BLACK) ? WHITE : BLACK;
  }

  // --- ゲーム終了処理 ---
  clear_screen();
  print_board(board);

  int black_count, white_count;
  count_pieces(board, &black_count, &white_count);

  printf("=== GAME OVER ===\n");
  printf("BLACK (X): %d\n", black_count);
  printf("WHITE (O): %d\n\n", white_count);

  if (black_count > white_count) {
    printf("Winner: BLACK (X)!\n");
  } else if (white_count > black_count) {
    printf("Winner: WHITE (O)!\n");
  } else {
    printf("Draw!\n");
  }

  return 0;
}
