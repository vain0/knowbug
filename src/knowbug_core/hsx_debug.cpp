#include "pch.h"
#include "hsx_internals.h"

namespace hsp_sdk_ext {
	void debug_do_update_location(HSP3DEBUG* debug) {
		debug->dbg_curinf();
	}

	auto debug_to_file_ref_name(HSP3DEBUG const* debug) -> std::optional<char const*> {
		auto file_ref_name = debug->fname;
		if (file_ref_name == nullptr || std::strcmp(file_ref_name, u8"???") == 0) {
			return std::nullopt;
		}
		return std::make_optional(file_ref_name);
	}

	auto debug_to_line_index(HSP3DEBUG const* debug) -> std::size_t {
		// 1-indexed。ただしファイルがないときは 0。
		auto line_number = debug->line;

		return (std::size_t)std::max(0, line_number - 1);
	}

	auto debug_to_general_info(HSP3DEBUG const* debug) -> std::unique_ptr<char, void(*)(char*)> {
		return std::unique_ptr<char, void(*)(char*)>{ debug->get_value(DEBUGINFO_GENERAL), debug->dbg_close };
	}

	auto debug_to_static_var_names(HSP3DEBUG const* debug) -> std::unique_ptr<char, void(*)(char*)> {
		return std::unique_ptr<char, void(*)(char*)>{ debug->get_varinf(nullptr, 0xFF), debug->dbg_close };
	}
}