#include "pch.h"
#include "hsx_internals.h"

namespace hsx {
	auto hsx_pval_to_type(PVal const* pval) -> HspType {
		assert(pval != nullptr);
		return (HspType)pval->flag;
	}

	auto hsx_pval_to_varmode(PVal const* pval) -> HspVarMode {
		assert(pval != nullptr);
		return (HspVarMode)pval->mode;
	}

	auto hsx_pval_to_lengths(PVal const* pval) -> HspDimIndex {
		assert(pval != nullptr);

		auto lengths = std::array<std::size_t, HspDimIndex::MAX_DIM>{};
		auto i = std::size_t{};

		while (i < HspDimIndex::MAX_DIM) {
			lengths[i] = pval->len[i + 1];

			if (i >= 1 && lengths[i] == 0) {
				break;
			}

			i++;
		}

		return HspDimIndex{ i, lengths };
	}

	auto hsx_pval_to_element_count(PVal const* pval) -> std::size_t {
		return hsx_pval_to_lengths(pval).size();
	}

	auto hsx_pval_is_standard_array(PVal const* pval, HSPCTX const* ctx) -> bool {
		assert(pval != nullptr);
		if (pval->len[1] == 1 && pval->len[2] == 0) {
			return false;
		}

		auto varproc = hsx_pval_to_varproc(pval, ctx);
		return hsx_varproc_does_support(varproc, HSPVAR_SUPPORT_FIXEDARRAY | HSPVAR_SUPPORT_FLEXARRAY);
	}

	auto hsx_pval_to_data(PVal const* pval, HSPCTX const* ctx) -> std::optional<HspData> {
		return hsx_element_to_data(pval, 0, ctx);
	}

	auto hsx_pval_to_memory_block(PVal const* pval, HSPCTX const* ctx) -> MemoryView {
		assert(pval != nullptr);

		auto&& data_opt = hsx_pval_to_data(pval, ctx);
		if (!data_opt) {
			return MemoryView{};
		}

		return hsx_element_data_to_memory_block(pval, data_opt->ptr(), ctx);
	}

	auto hsx_pval_to_str(PVal const* pval, HSPCTX const* ctx)->std::optional<HspStr> {
		return hsx_element_to_str(pval, 0, ctx);
	}
}
