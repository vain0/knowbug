#pragma once

#include "hsx.h"

// 危険な操作に印をつける。
#define UNSAFE(E) (E)

namespace hsp_sdk_ext {
	static auto data(HSPCTX const* ctx)->Slice<char> {
		auto size = (std::size_t)std::max(0, ctx->hsphed->max_ds) / sizeof(char);
		return Slice<char>{ ctx->mem_mds, size };
	}

	static auto data_as_str(std::size_t ds_index, HSPCTX const* ctx)->std::optional<char const*> {
		if (ds_index >= data(ctx).size()) {
			return std::nullopt;
		}

		return data(ctx).begin() + ds_index;
	}

	static auto exinfo(HSPCTX* ctx)->HSPEXINFO* {
		assert(ctx != nullptr);
		return ctx->exinfo2;
	}

	static auto exinfo(HSPCTX const* ctx)->HSPEXINFO const* {
		return exinfo(const_cast<HSPCTX*>(ctx));
	}

	static auto vartype_to_int(HspType vartype)->int {
		return (int)vartype;
	}

	static auto vartype_from_int(int vartype)->HspType {
		return (HspType)vartype;
	}

	static auto vartype_to_varproc(HspType vartype, HSPCTX const* ctx)->HspVarProc* {
		// FIXME: vartype の範囲検査 (ランタイム側は範囲検査しない)
		return exinfo(ctx)->HspFunc_getproc(vartype_to_int(vartype));
	}

	// support_flag: HSPVAR_SUPPORT_*
	static auto varproc_does_support(HspVarProc const* varproc, unsigned short support_flag)->bool {
		assert(varproc != nullptr);
		return (varproc->support & support_flag) != 0;
	}

	static auto pval_to_varproc(PVal const* pval, HSPCTX const* ctx)->HspVarProc const* {
		return vartype_to_varproc(pval_to_type(pval), ctx);
	}

	// pdat は pval に含まれる要素の実体ポインタとする。
	static auto element_data_to_memory_block(PVal const* pval, PDAT const* pdat, HSPCTX const* ctx)->MemoryView {
		assert(pval != nullptr && pdat != nullptr);

		auto varproc = pval_to_varproc(pval, ctx);

		int buffer_size;
		auto data = varproc->GetBlockSize(const_cast<PVal*>(pval), const_cast<PDAT*>(pdat), &buffer_size);

		if (buffer_size <= 0 || data == nullptr) {
			return MemoryView{};
		}

		return MemoryView{ data, (std::size_t)buffer_size };
	}

	static auto data_from_int_opt(std::optional<HspInt const*> ptr_opt)->std::optional<HspData> {
		if (!ptr_opt) {
			return std::nullopt;
		}

		return std::make_optional(data_from_int(*ptr_opt));
	}
}