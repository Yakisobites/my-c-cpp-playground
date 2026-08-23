/**
 * @file reversi.h
 * @brief Reversiゲームのデータ構造と関数の宣言
 */
#ifndef REVERSI_H
#define REVERSI_H
#define BOARD_SIZE 8
#define TOTAL_CELLS (BOARD_SIZE * BOARD_SIZE)

/**
 * @brief 真偽値を表す列挙型
 */
typedef enum { FALSE, TRUE } Bool;
/**
 * @brief セルの状態を表す列挙型
 */
typedef enum { EMPTY, BLACK, WHITE } CellState;
/**
 * @brief ゲームの手の結果を表す列挙型
 */
typedef enum {
  MOVE_SUCCESS,
  MOVE_INVALID,
  MOVE_NO_FLIP,
  MOVE_OUT_OF_BOUNDS,
  MOVE_CELL_NOT_EMPTY
} MoveResult;
/**
 * @brief ゲームの結果を表す列挙型
 */
typedef enum { GAME_CONTINUE, BLACK_WIN, WHITE_WIN, DRAW } GameResult;
/**
 * @brief ゲームの結果の詳細を表す構造体
 */
typedef struct {
  GameResult result;
  int black_count;
  int white_count;
} GameResultDetails;
/**
 * @brief ゲームの状態を表す構造体
 */
typedef struct {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  CellState current_player;
  int consecutive_passes;
} GameState;

/**
 * @brief 盤上の位置を表す構造体
 */
typedef struct {
  int row;
  int col;
} Position;

/**
 * @brief 方向を表す構造体
 */
typedef struct {
  int row_delta;
  int col_delta;
} Direction;
/**
 * @brief 移動可能な位置のリストを格納するための構造体
 */
typedef struct {
  Position moves[TOTAL_CELLS];
  int count;
} PositionList;

/**
 * @brief ゲームの初期化と操作に関する関数の宣言
 * @param game ゲームの状態を表す構造体へのポインタ
 * @return なし
 * @note
 * この関数は、ゲームの初期状態を設定します。盤面を空にし、中央に初期の石を配置し、現在のプレイヤーを黒に設定します。
 */
void init_game(GameState *game);
/**
 * @brief 指定された位置が盤面の範囲外かどうかを判定する関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @param pos 判定する位置
 * @return 範囲外であればTRUE、そうでなければFALSE
 */
Bool is_out_of_bounds(const GameState *game, Position pos);
/**
 * @brief 指定された位置が空でないかどうかを判定する関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @param pos 判定する位置
 * @return 空でなければTRUE、空であればFALSE
 */
Bool is_not_empty(const GameState *game, Position pos);
/**
 * @brief 指定された位置が有効な手かどうかを判定する関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @param pos 判定する位置
 * @return 有効な手であればTRUE、そうでなければFALSE
 */
Bool is_valid_move(const GameState *game, Position pos);
/**
 * @brief 指定された位置に石を置く関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @param pos 石を置く位置
 * @return 手の結果
 * @note
 * この関数は、指定された位置に石を置き、ひっくり返せる相手の石をすべてひっくり返します。
 */
MoveResult game_place_piece(GameState *game, Position pos);

/**
 * @brief 指定されたゲーム状態から有効な手のリストを取得する関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @return 有効な手のリスト
 */
PositionList game_get_valid_moves(const GameState *game);
/**
 * @brief パスを行う関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @return パスが成功すればTRUE、そうでなければFALSE
 */
Bool game_pass(GameState *game);
/**
 * @brief 指定されたゲームの結果の詳細を取得する関数
 * @param game ゲームの状態を表す構造体へのポインタ
 * @return ゲームの結果の詳細
 */
GameResultDetails game_get_result(const GameState *game);
/**
 * @brief ランダムに有効な手を選択する関数
 * @param list 有効な手のリストへのポインタ
 * @return 選択された位置
 */
Position select_random_move(const PositionList *list);
/**
 * @brief Reversiゲームをプレイする関数
 * @note
 * ターミナル上でReversiゲームをプレイするためのメインループを実行します。プレイヤーとAIが交互に手を打ち、ゲームの結果が決定されるまで続きます。
 * 現在は、プレイヤーの手はランダムに選択されるようになっています。
 */
int play_reversi(void);

#endif // REVERSI_H
