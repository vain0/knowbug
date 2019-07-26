#pragma once

#include <vector>
#include "encoding.h"

// HSP のスクリプトファイルを表す。
class SourceFile {
	OsString full_path_;

	Utf8String full_path_as_utf8_;

	bool loaded_;

	Utf8String content_;

	std::vector<Utf8StringView> lines_;

public:
	SourceFile(OsString&& full_path);

	auto full_path() const -> OsStringView {
		return as_view(full_path_);
	}

	auto full_path_as_utf8() const -> Utf8StringView {
		return as_view(full_path_as_utf8_);
	}

	auto content() -> Utf8StringView;

	// 指定した行の文字列 (字下げを除く) を取得する。
	auto line_at(std::size_t line_index) -> std::optional<Utf8StringView>;

private:
	void load();
};
