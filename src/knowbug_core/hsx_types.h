#ifndef HSX_TYPES_H
#define HSX_TYPES_H

#include <stdint.h>

// code segment (CS) 領域のデータ単位。
typedef uint16_t HsxCodeUnit;

// オブジェクトテンポラリ (ラベルのこと)
// code segment (CS) 領域のオフセットという形で、コードの位置を表す。
typedef int32_t HsxObjectTemp;

// HSP の label 型の値。
// ランタイムにおけるラベルの表現。
// code segment 内へのポインタ (有効) または nullptr (無効)。
typedef HsxCodeUnit const* HsxLabel;

// HSP の str 型の値。
//
// 1. str 型はランタイムエンコーディング (shift_jis/utf-8) の文字列だけでなく、
//    他のエンコーディングの文字列や任意のバイナリを格納するのにも使われることがたまにある。
//    特に、null 終端とは限らない点に注意。std::strlen などの null 終端を前提とする関数に渡してはいけない。
// 2. 変数や refstr に由来する文字列データはバッファサイズが容易に取得できる。
//    str 引数の文字列データはバッファサイズを取得できないが、null 終端が保証されている。
typedef PDAT const* HsxStr;

// HSP の double 型の値。
typedef double HsxDouble;

// HSP の int 型の値。
typedef int32_t HsxInt;

// 変数の型の番号。
// 定数 HSPVAR_FLAG_* の値が入る。
// HspVarProc::flag や PVal::flag などに格納されている。
// HSP の vartype 関数の値でもある。
typedef int32_t HsxVarType;

// 変数の状態。
// 定数 HSPVAR_MODE_* の値が入る。
//
// - CLONE: dup などで作られたクローン変数の状態
typedef int32_t HsxVarMode;

// #deffunc や #func などの引数の種類。
// 定数 MP_TYPE_* の値が入る。
typedef int32_t HsxParamType;

// デバッガーへの通知の種類。
typedef enum HsxDebugNotice {
	// assert, stop やステップ実行の完了などにより、HSP スクリプトの実行が一時停止したとき
	HSX_DEBUG_NOTICE_STOP = 0,

	// logmes 命令が実行されたとき。ログの内容は ctx->stmp にある。
	HSX_DEBUG_NOTICE_LOGMES = 1,
} HsxDebugNotice;

// メモリ領域の一部へのポインタ。
typedef struct HsxSlice {
	void const* data;
	int32_t size;
} HsxSlice;

// 配列へのインデックス。(4次元ベクトル。)
typedef struct HsxArrayIndex {
	// 次元数
	int32_t dim;

	// 各次元のインデックス
	int32_t indexes[4];
} HsxArrayIndex;

// 配列要素を要素番号で指すポインタ。
typedef struct HsxElementPtr {
	PVal const* pval;

	// 要素番号
	int32_t aptr;
} HsxElementPtr;

// 配列要素を配列インデックスで指すポインタ。
typedef struct HsxElementIndex {
	PVal const* pval;
	HsxArrayIndex index;
} HsxElementIndex;

// HSP の値へのポインタ。
typedef struct HsxValuePtr {
	HsxVarType var_type;
	PDAT const* data_ptr;
} HsxValuePtr;

// パラメータスタック。
// ユーザー定義命令・関数の引数や、モジュール変数のインスタンスデータの管理領域。
typedef struct HsxParamStack {
	// ユーザー定義命令・関数の管理情報。
	// またはモジュール変数の基となるモジュールの管理情報。
	STRUCTDAT const* struct_data;

	// 実データ領域。HSPCTX::prmstack の参照先。
	void const* stack_ptr;

	int32_t stack_size;
} HsxParamStack;

// パラメータへのポインタ。
typedef struct HsxParamPtr {
	HsxParamType param_type;

	// 実データへのポインタ。
	// パラメータスタックの一部。
	// 指す先の内容は param_type による。
	void const* param_ptr;

	int32_t param_size;
} HsxParamPtr;

#endif
