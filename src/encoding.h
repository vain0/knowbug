//! 文字列のエンコーディング関連

#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <string_view>
#include <Windows.h>
#include <tchar.h>

// shift_jis エンコーディングされた文字列の要素 (バイト)
enum class SjisChar
	: char
{
};

// UTF-8 エンコーディングされた文字列の要素 (バイト)
// NOTE: char8_t が存在しなかった時代のため
enum class Utf8Char
	: char
{
};

// Windows API のための文字列。
// UNICODE 版なら utf-16、そうでなければ ANSI (shift_jis)。
using OsString = std::basic_string<TCHAR>;

// Windows API のための文字列への参照。
using OsStringView = std::basic_string_view<TCHAR>;

// shift_jis (cp932) でエンコーディングされた文字列。
using SjisString = std::basic_string<SjisChar>;

// shift_jis 文字列への参照
using SjisStringView = std::basic_string_view<SjisChar>;

// UTF-8 でエンコーディングされた文字列
using Utf8String = std::basic_string<Utf8Char>;

// UTF-8 文字列への参照
using Utf8StringView = std::basic_string_view<Utf8Char>;

#ifdef HSP3_UTF8

// HSP が使用するエンコーディングの文字列。
using HspString = Utf8String;

// HSP が使用するエンコーディングの文字列への参照。
using HspStringView = Utf8StringView;

#else

// HSP が使用するエンコーディングの文字列。
using HspString = SjisString;

// HSP が使用するエンコーディングの文字列への参照。
using HspStringView = SjisStringView;

#endif

extern auto string_is_ascii(char const* str) -> bool;

extern auto ascii_as_utf8(char const* source) -> Utf8StringView;

extern auto ascii_to_utf8(std::string&& source) -> Utf8String;

extern auto ascii_to_utf8(std::string const& source) -> Utf8String;

extern auto as_hsp(char const* str) -> HspStringView;

extern auto as_hsp(std::string&& str) -> HspString;

extern auto to_hsp(OsStringView const& source) -> HspString;

extern auto to_hsp(SjisStringView const& source) -> HspString;

extern auto to_hsp(Utf8StringView const& source) -> HspString;

extern auto as_os(LPCTSTR str) -> OsStringView;

extern auto to_os(SjisStringView const& source) -> OsString;

extern auto to_os(Utf8StringView const& source) -> OsString;

extern auto as_sjis(char const* str) -> SjisStringView;

extern auto to_sjis(OsStringView const& source) -> SjisString;

extern auto to_sjis(Utf8StringView const& source) -> SjisString;

extern auto as_utf8(char const* str) -> Utf8StringView;

extern auto to_utf8(OsStringView const& source) -> Utf8String;

extern auto to_utf8(SjisStringView const& source) -> Utf8String;

extern auto to_owned(OsStringView const& source) -> OsString;

extern auto to_owned(SjisStringView const& source) -> SjisString;

extern auto to_owned(Utf8StringView const& source) -> Utf8String;

extern auto as_view(OsString const& source) -> OsStringView;

extern auto as_view(SjisString const& source) -> SjisStringView;

extern auto as_view(Utf8String const& source) -> Utf8StringView;

extern auto as_native(SjisStringView const& source) -> char const*;

extern auto as_native(Utf8StringView const& source) -> char const*;

extern auto as_native(SjisString&& source) -> std::string;

extern auto as_native(Utf8String&& source) -> std::string;

namespace std {
	template<>
	struct hash<OsString> {
		auto operator ()(OsString const& str) const -> std::size_t {
			return std::hash<std::basic_string<TCHAR>>{}(str);
		}
	};
}
