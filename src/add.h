/**
 * @file add.h
 * @brief 数値計算に関する関数群の宣言ヘッダー
 */
#ifndef ADD_H
#define ADD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 2つの整数を加算します。
 *
 * @param a 加算する1つ目の整数
 * @param b 加算する2つ目の整数
 * @return int a と b の和 (a + b)
 * @note
 * この関数は、整数の加算を行う基本的な関数です。オーバーフローのチェックは行いませんので、使用する際には注意してください。
 */
int add(int a, int b);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADD_H
