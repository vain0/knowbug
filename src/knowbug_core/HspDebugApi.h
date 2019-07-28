#pragma once

#include <optional>
#include <string>
#include "../hpiutil/hpiutil_fwd.hpp"
#include "HspTypes.h"

namespace hpiutil {
	class DInfo;
}

// HSP SDK のラッパー
// 配列アクセスの範囲検査、null 検査、整数と enum の変換など、小さい仕事をする。
class HspDebugApi {
public:
	class ModuleStruct;
	class SourceLocation;

private:
	HSPCTX* context_;

	HSP3DEBUG* debug_;

	HSPEXINFO* exinfo_;

public:
	HspDebugApi(HSP3DEBUG* debug);

	auto context() -> HSPCTX* {
		return context_;
	}

	auto context() const -> HSPCTX const* {
		return context_;
	}

	auto debug() -> HSP3DEBUG* {
		return debug_;
	}

	auto debug() const -> HSP3DEBUG const* {
		return debug_;
	}

	auto exinfo() -> HSPEXINFO* {
		return exinfo_;
	}

	auto exinfo() const -> HSPEXINFO const* {
		return exinfo_;
	}

	auto current_file_ref_name() const->std::optional<char const*>;

	auto current_line() const->std::size_t;

	auto static_vars() const->PVal const*;

	auto static_var_count() const->std::size_t;

	auto static_var_find_by_name(char const* var_name) const->std::optional<std::size_t>;

	auto static_var_find_name(std::size_t static_var_id) const->std::optional<std::string>;

	auto static_var_to_pval(std::size_t static_var_id) const->PVal const*;

	auto var_to_type(PVal const* pval) const->HspType;

	auto var_to_data(PVal const* pval) const->HspData;

	auto var_to_lengths(PVal const* pval) const->HspDimIndex;

	auto var_to_mode(PVal const* pval) const->HspVarMode;

	bool var_is_array(PVal const* pval) const;

	auto var_to_element_count(PVal const* pval) const->std::size_t;

	auto var_element_to_indexes(PVal const* pval, std::size_t aptr) const->std::optional<HspDimIndex>;

	auto var_element_to_aptr(PVal const* pval, HspDimIndex const& indexes) const->std::optional<std::size_t>;

	auto var_element_to_data(PVal const* pval, std::size_t aptr) const->HspData;

	auto var_to_block_memory(PVal const* pval) const->MemoryView;

	auto var_element_to_block_memory(PVal const* pval, std::size_t aptr) const->MemoryView;

	auto system_var_to_data(HspSystemVarKind system_var_kind) const->std::optional<HspData>;

	auto data_to_label(HspData const& data) const->HspLabel;

	auto data_to_str(HspData const& data) const->HspStr;

	auto data_to_double(HspData const& data) const->HspDouble;

	auto data_to_int(HspData const& data) const->HspInt;

	auto data_to_flex(HspData const& data) const->FlexValue const*;

	auto static_label_count() const->std::size_t;

	auto static_label_to_label(std::size_t static_label_id) const->std::optional<HspLabel>;

	bool flex_is_nullmod(FlexValue const* flex) const;

	bool flex_is_clone(FlexValue const* flex) const;

	auto flex_to_module_struct(FlexValue const* flex) const->STRUCTDAT const*;

	auto flex_to_module_tag(FlexValue const* flex) const->STRUCTPRM const*;

	auto flex_to_member_count(FlexValue const* flex) const->std::size_t;

	auto flex_to_member_at(FlexValue const* flex, std::size_t index) const->HspParamData;

	auto flex_to_param_stack(FlexValue const* flex) const->HspParamStack;

	auto struct_to_name(STRUCTDAT const* struct_dat) const -> char const*;

	auto struct_to_param_count(STRUCTDAT const* struct_dat) const->std::size_t;

	auto struct_to_param_at(STRUCTDAT const* struct_dat, std::size_t param_index) const->std::optional<STRUCTPRM const*>;

	auto struct_to_param_stack_size(STRUCTDAT const* struct_dat) const->std::size_t;

	auto params() const->STRUCTPRM const*;

	auto param_count() const->std::size_t;

	auto param_to_param_id(STRUCTPRM const* param) const->std::size_t;

	auto param_to_name(STRUCTPRM const* param, std::size_t param_index, hpiutil::DInfo const& debug_segment) const->std::string;

	auto param_stack_to_data_count(HspParamStack const& param_stack) const->std::size_t;

	auto param_stack_to_data_at(HspParamStack const& param_stack, std::size_t param_index) const->std::optional<HspParamData>;

	auto param_type_to_name(HspParamType param_type) const -> char const*;

	auto param_data_to_type(HspParamData const& param_data) const->HspParamType;

	auto param_data_as_local_var(HspParamData const& param_data) const->PVal const*;

	auto param_data_to_single_var(HspParamData const& param_data) const->MPVarData const*;

	auto param_data_to_array_var(HspParamData const& param_data) const->MPVarData const*;

	auto param_data_to_mod_var(HspParamData const& param_data) const->std::optional<MPModVarData const*>;

	auto param_data_to_data(HspParamData const& param_data) const->std::optional<HspData>;
};