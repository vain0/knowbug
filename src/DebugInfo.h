﻿#ifndef IG_STRUCT_DEBUG_INFO_H
#define IG_STRUCT_DEBUG_INFO_H

#include <memory>
#include <vector>
#include "hsp3debug.h"
#include "hsp3struct.h"
#include "hspvar_core.h"

namespace hpimod { class CAx; }

// HSP3DEBUG wrapper
// with CAx
class DebugInfo
{
	using string = std::string;

	HSP3DEBUG* const debug_;
	std::unique_ptr<hpimod::CAx> const ax_;

public:
	DebugInfo(HSP3DEBUG* debug);
	~DebugInfo();

	hpimod::CAx const& getAx() const { return *ax_; }

	bool setStepMode(int mode) { return (debug_->dbg_set(mode) >= 0); }

	std::vector<std::pair<string, string>> fetchGeneralInfo() const;
	std::vector<string> fetchStaticVarNames() const;

	// current position data
	char const* curFileName() const {
		return debug_->fname;
	}
	int curLine() const {
		return debug_->line - 1;
	}
	string getCurInfString() const {
		return formatCurInfString(curFileName(), curLine());
	}
	static string formatCurInfString(char const* fname, int line);

	void updateCurInf() {
		debug_->dbg_curinf();
	}
};

extern std::unique_ptr<DebugInfo> g_dbginfo;

#endif