# Reversi モジュール仕様

このドキュメントは、`src/reversi.h` と `src/reversi.c` で実装されている Reversi（オセロ）処理の仕様を整理したものです。

## 1. 概要

- 盤面サイズは `8 x 8` 固定です。
- ゲームは黒と白の2人対戦形式です。
- 盤面の各セルは `EMPTY` / `BLACK` / `WHITE` の3状態を持ちます。
- 1手の合法判定は、相手の石を挟んで自分の石へ変換できるかで決まります。
- 実装上の勝敗判定は、`consecutive_passes` が 2 以上になった時点で集計を行います。
- 現在の `play_reversi()` は、プレイヤー手をランダム選択の簡易実装として扱っています。

---

## 2. 定数と基本型

### 2.1 定数

```c
#define BOARD_SIZE 8
#define TOTAL_CELLS (BOARD_SIZE * BOARD_SIZE)
```

- `BOARD_SIZE`: 盤面の1辺のサイズ
- `TOTAL_CELLS`: 盤面上の全セル数（64）

### 2.2 真偽値

```c
typedef enum { FALSE, TRUE } Bool;
```

- `FALSE` = 0
- `TRUE` = 1

### 2.3 セル状態

```c
typedef enum { EMPTY, BLACK, WHITE } CellState;
```

| 定数 | 値 | 説明 |
| --- | --- | --- |
| `EMPTY` | 0 | 空きマス |
| `BLACK` | 1 | 黒石 |
| `WHITE` | 2 | 白石 |

### 2.4 手の結果

```c
typedef enum {
  MOVE_SUCCESS,
  MOVE_INVALID,
  MOVE_NO_FLIP,
  MOVE_OUT_OF_BOUNDS,
  MOVE_CELL_NOT_EMPTY
} MoveResult;
```

| 定数 | 説明 |
| --- | --- |
| `MOVE_SUCCESS` | 手が正常に置けた |
| `MOVE_INVALID` | 一般的な不正手 |
| `MOVE_NO_FLIP` | 相手石を挟めず、石を置けなかった |
| `MOVE_OUT_OF_BOUNDS` | 盤外の位置を指定した |
| `MOVE_CELL_NOT_EMPTY` | 既に石が置かれているマス |

> `MOVE_INVALID` は定義されているものの、現行の実装では実際には使用されていません。

### 2.5 ゲーム結果

```c
typedef enum { GAME_CONTINUE, BLACK_WIN, WHITE_WIN, DRAW } GameResult;
```

| 定数 | 説明 |
| --- | --- |
| `GAME_CONTINUE` | ゲーム継続 |
| `BLACK_WIN` | 黒の勝利 |
| `WHITE_WIN` | 白の勝利 |
| `DRAW` | 引き分け |

---

## 3. 主要データ構造

### 3.1 ゲーム状態

```c
typedef struct {
  CellState board[BOARD_SIZE][BOARD_SIZE];
  CellState current_player;
  int consecutive_passes;
} GameState;
```

- `board`: 8x8 の盤面
- `current_player`: 現在の手番（`BLACK` または `WHITE`）
- `consecutive_passes`: 連続パス回数

初期状態では以下の4マスに石が置かれます。

- `(3, 3) = WHITE`
- `(4, 4) = WHITE`
- `(3, 4) = BLACK`
- `(4, 3) = BLACK`

### 3.2 位置

```c
typedef struct {
  int row;
  int col;
} Position;
```

- `row`: 行番号（0〜7）
- `col`: 列番号（0〜7）

### 3.3 方向

```c
typedef struct {
  int row_delta;
  int col_delta;
} Direction;
```

- 8方向を表すオフセットとして利用される。
- 例: `(1, 0)`, `(-1, 1)`, `(0, -1)` など。

### 3.4 有効手リスト

```c
typedef struct {
  Position moves[TOTAL_CELLS];
  int count;
} PositionList;
```

- `moves`: 有効な手の一覧
- `count`: 有効な手の数

### 3.5 ゲーム結果詳細

```c
typedef struct {
  GameResult result;
  int black_count;
  int white_count;
} GameResultDetails;
```

- 最終結果と石の枚数をまとめて保持する。

---

## 4. 公開関数一覧

### 4.1 `void init_game(GameState *game);`

#### 役割

初期状態の盤面を作成し、プレイヤーと連続パス状態を初期化する。

#### 振る舞い

- 盤面全体を `EMPTY` に初期化
- 中央4マスに初期配置
- `current_player = BLACK`
- `consecutive_passes = 0`

### 4.2 `Bool is_out_of_bounds(const GameState *game, Position pos);`

#### 役割

指定した位置が盤面範囲外かを判定する。

#### 戻り値

- 範囲外: `TRUE`
- 範囲内: `FALSE`

#### 判定条件

```c
pos.row < 0 || pos.row >= BOARD_SIZE || pos.col < 0 || pos.col >= BOARD_SIZE
```

### 4.3 `Bool is_not_empty(const GameState *game, Position pos);`

#### 役割

指定位置にすでに石が存在するか判定する。

#### 戻り値

- 空きマスでない: `TRUE`
- 空きマス: `FALSE`

### 4.4 `Bool is_valid_move(const GameState *game, Position pos);`

#### 役割

指定位置が現時点で合法手か判定する。

#### 実装方法

- `GameState temp = *game;` を作成
- `game_place_piece(&temp, pos)` を実行
- `MOVE_SUCCESS` が返れば `TRUE` を返す

#### 注意

この判定は、盤面を一時コピーして置けるかどうかを確認しているため、動作上は「仮置き検証」として実装されている。

### 4.5 `MoveResult game_place_piece(GameState *game, Position pos);`

#### 役割

指定位置に石を置き、挟まれた相手石を自石に変換する。

#### 検査順序

1. `pos` が盤外か判定
2. そのマスが空いているか判定
3. 8方向を走査し、相手石を挟めるか確認
4. 挟める石が1つでもあれば配置し、変換する
5. `current_player` を相手に切り替える

#### 返却値

- `MOVE_SUCCESS`: 置けて変換した
- `MOVE_OUT_OF_BOUNDS`: 盤外
- `MOVE_CELL_NOT_EMPTY`: 既に石あり
- `MOVE_NO_FLIP`: 相手石を挟めない

#### 合法手の定義

実装では、置いた位置から各方向へ進み、相手の石を続けた後に自分の石に到達する場合に合法手とみなす。

### 4.6 `PositionList game_get_valid_moves(const GameState *game);`

#### 役割

現在のゲーム状態における有効な手の一覧を取得する。

#### 戻り値

- `PositionList` 構造体
- `list.count` に有効手数が入る
- `list.moves[i]` に各座標が格納される

#### 実装ルール

- 盤面全体を走査
- 各セルに対して `can_place()` を呼び出す
- 合法手であれば `moves` 配列に追加

### 4.7 `Bool game_pass(GameState *game);`

#### 役割

現在のプレイヤーが有効手を持たない場合にパスを実行する。

#### 振る舞い

- `game_get_valid_moves(game)` で有効手を確認
- 0件なら `consecutive_passes++`
- 手番を相手に切り替える
- `TRUE` を返す

#### 返却値

- 有効手が存在し、パス不可: `FALSE`
- パス成功: `TRUE`

### 4.8 `GameResultDetails game_get_result(const GameState *game);`

#### 役割

現在の盤面を集計し、ゲーム結果を返す。

#### 判定ロジック

```c
if (game->consecutive_passes < 2) {
  result.result = GAME_CONTINUE;
  result.black_count = 0;
  result.white_count = 0;
  return result;
}
```

- `consecutive_passes < 2` の場合は、ゲームは継続とみなす
- 2回連続パスをした後にのみ枚数を数えて勝敗を確定する

#### 勝敗判定

- 黒石数 > 白石数: `BLACK_WIN`
- 白石数 > 黒石数: `WHITE_WIN`
- 同数: `DRAW`

### 4.9 `Position select_random_move(const PositionList *list);`

#### 役割

有効手候補の中からランダムに1つ選ぶ。

#### 戻り値

- 有効手がある場合: ランダム選択した `Position`
- 0件の場合: `{-1, -1}` を返す

#### 備考

`rand()` を用いて乱択するため、ゲームの実戦ではなくテストや簡易AI向けの挙動となる。

### 4.10 `int play_reversi(void);`

#### 役割

ゲームのメインループを実行する。

#### 流れ

1. `GameState game;` を初期化
2. `while (game_get_result(&game).result == GAME_CONTINUE)` を続行
3. 有効手が存在しない場合は自動パス
4. 盤面を表示
5. 現在の手番に応じて手を選ぶ
6. `game_place_piece()` を呼び出す
7. ゲーム終了後に結果表示

#### 現在の実装の特徴

- 黒は「プレイヤー」だが、実際には入力処理ではなくランダム手選択を使用
- 白もランダム選択
- `sleep_ms(500)` により、手の間に短い待ち時間を挿入している

---

## 5. 内部ヘルパ関数

以下の関数は `reversi.c` 内に定義されており、ヘッダファイルには宣言されていない。

### 5.1 `static void clear_screen(void)`

- ANSI エスケープコードを使って画面をクリアする

### 5.2 `static void print_board(GameState *game)`

- 盤面をコンソールに表示する
- `X` は黒、`O` は白、`.` は空きマス

### 5.3 `static CellState get_opponent(CellState player)`

- 対戦相手の色を返す
- `BLACK -> WHITE`, `WHITE -> BLACK`

### 5.4 `static int flip_direction(GameState *game, Position pos, Direction dir)`

- ある方向に進み、相手石を挟んで自石に変えられるかを確認し、該当石を変換する
- 返却値は反転した枚数

### 5.5 `static Bool can_place(const GameState *game, Position pos)`

- `game_place_piece()` の実行可否を確認する
- `PositionList` を作る際に利用される

### 5.6 `static Position get_user_input(void)`

- 入力待ちの補助関数
- 実装上は未使用（プレーンのランダム選択が採用されている）

### 5.7 `static void display_results(const GameState *game)`

- 最終結果を表示する

---

## 6. 制約・注意事項

- 盤面は固定サイズの `8 x 8` であり、動的変更はできない。
- `game_place_piece()` は「相手石を挟んで初めて置ける」仕様である。
- 置いたマスが空でない場合は `MOVE_CELL_NOT_EMPTY` を返す。
- 盤外座標は `MOVE_OUT_OF_BOUNDS` を返す。
- `game_get_result()` が `GAME_CONTINUE` を返すのは、`consecutive_passes < 2` までである。
- 乱択関数 `select_random_move()` は、同一の有効手を毎回ランダムに選ぶため、戦略性はない。
- 実装上の `play_reversi()` はデモ用途であり、対人入力やAI強化のロジックは未実装である。

---

## 7. 例: 初期盤面の状態

```c
GameState game;
init_game(&game);

// 初期配置
// (3,3) = WHITE
// (4,4) = WHITE
// (3,4) = BLACK
// (4,3) = BLACK
// current_player = BLACK
/*
  --- Reversi ---
    0 1 2 3 4 5 6 7
  0 . . . . . . . .
  1 . . . . . . . .
  2 . . . . . . . .
  3 . . . O X . . .
  4 . . . X O . . .
  5 . . . . . . . .
  6 . . . . . . . .
  7 . . . . . . . .
*/
```

---

## 8. まとめ

`reversi.h` / `reversi.c` は、Reversi の盤面管理、合法手判定、石の配置、勝敗判定、パス処理、簡易ゲームループの基本機能を提供するモジュールである。
実装はシンプルな配列ベースで、C 言語の標準的な手法で構成されている。
