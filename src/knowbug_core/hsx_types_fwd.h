// hsx の型
// HSPSDK の　hspvar_core を include しないとコンパイルできない。

#ifndef HSX_TYPES_FWD_H
#define HSX_TYPES_FWD_H

#include "hsx_types.h"

namespace hsx {
	class HspData;
	class HspDimIndex;
	class HspParamData;
	class HspParamStack;
	class HspVarMetadata;

	using HspCodeUnit = HsxCodeUnit;
	using HspObjectTemp = HsxObjectTemp;
	using HspLabel = HsxLabel;
	using HspDouble = HsxDouble;
	using HspInt = HsxInt;
	using HspParamType = HsxParamType;

	// HSP の変数が持つデータの型
	enum class HspType {
		None = HSPVAR_FLAG_NONE,
		Label = HSPVAR_FLAG_LABEL,
		Str = HSPVAR_FLAG_STR,
		Double = HSPVAR_FLAG_DOUBLE,
		Int = HSPVAR_FLAG_INT,
		Struct = HSPVAR_FLAG_STRUCT,
		Comstruct = HSPVAR_FLAG_COMSTRUCT,
	};

	// HSP の変数のモード
	enum class HspVarMode {
		None = HSPVAR_MODE_NONE,
		Alloc = HSPVAR_MODE_MALLOC,
		Clone = HSPVAR_MODE_CLONE,
	};

	// システム変数の種類。
	// 順番は名前順、ただし類似したものは近くに集める。
	enum class HspSystemVarKind {
		Cnt = 1,
		Err,
		IParam,
		WParam,
		LParam,
		LoopLev,
		SubLev,
		Refstr,
		Refdval,
		Stat,
		StrSize,
		Thismod,
	};

	// デバッグウィンドウへの通知の種類
	enum class DebugNoticeKind {
		// assert, stop やステップ実行の完了などにより、HSP スクリプトの実行が一時停止したとき
		Stop = HSX_DEBUG_NOTICE_STOP,

		// logmes 命令が実行されたとき。ログの内容は ctx->stmp にある。
		Logmes = HSX_DEBUG_NOTICE_LOGMES,
	};
}

#endif
