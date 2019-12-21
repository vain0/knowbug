#include "pch.h"
#include "hsx_data.h"
#include "hsx_internals.h"

namespace hsx {
	auto hsx_structs(HSPCTX const* ctx)->Slice<STRUCTDAT> {
		auto size = (std::size_t)std::max(0, ctx->hsphed->max_finfo) / sizeof(STRUCTDAT);
		return Slice<STRUCTDAT>{ ctx->mem_finfo, size };
	}

	auto hsx_struct_to_name(STRUCTDAT const* struct_dat, HSPCTX const* ctx)->std::optional<char const*> {
		return hsx_data_segment_to_str((std::size_t)std::max(0, struct_dat->nameidx), ctx);
	}

	auto hsx_struct_to_param_count(STRUCTDAT const* struct_dat)->std::size_t {
		return (std::size_t)std::max(0, struct_dat->prmmax);
	}

	auto hsx_struct_to_params(STRUCTDAT const* struct_dat, HSPCTX const* ctx)->Slice<STRUCTPRM> {
		auto offset = (std::size_t)std::max(0, struct_dat->prmindex);
		auto count = hsx_struct_to_param_count(struct_dat);
		return hsx_params(ctx).slice(offset, count);
	}

	auto hsx_struct_to_param_stack_size(STRUCTDAT const* struct_dat)->std::size_t {
		return (std::size_t)std::max(0, struct_dat->size);
	}
}
