﻿// 変数データの文字列を作るクラス

#ifndef IG_CLASS_VARDATA_STRING_H
#define IG_CLASS_VARDATA_STRING_H

#include <stack>

#include "main.h"
#include "DebugInfo.h"
#include "SysvarData.h"
#include "module/strf.h"
#include "module/CStrWriter.h"

#include "CVarTree.h"

#if defined(with_Assoc) || defined(with_Vector) || defined(with_Array)
class CAssoc;
class CVector;
class CArray;
#endif
#ifdef with_WrapCall
namespace WrapCall
{
	struct ResultNodeData;
} // namespace WrapCall
#endif

class CVardataStrWriter
{
	// メンバ変数
private:
	// 継承してもいいが、ここでは包含にしておく
	std::unique_ptr<CStructedStrWriter> writer_;

	// メンバ関数
public:
	template<typename TWriter>
	static CVardataStrWriter create(string* buf)
	{
		return CVardataStrWriter(buf, static_cast<TWriter*>(nullptr));
	}
private:
	template<typename TWriter>
	CVardataStrWriter(string* buf,  TWriter* /* for definite template parameter */)
		: writer_(static_cast<CStructedStrWriter*>(new TWriter(buf)))
	{ }

public:
	string const& getString() const { return getWriter().get(); }

	void addVar(char const* name, PVal const* pval);
	void addVarScalar(char const* name, PVal const* pval);
	void addVarScalar(char const* name, PVal const* pval, APTR aptr);
	void addVarArray(char const* name, PVal const* pval);
	void addVarArrayRec(PVal const* pval, size_t const cntElem[], size_t idxDim, APTR aptr_offset);

	void addValue(char const* name, vartype_t type, PDAT const* ptr);
	void addValueStruct(char const* name, FlexValue const* fv);

#ifdef with_Assoc
	void addItem_assoc(char const* name, CAssoc* src);
#endif
#ifdef with_Vector
	void addItem_vector(char const* name, CVector* src);
#endif
#ifdef with_Array
	void addItem_array(char const* name, CArray* src);
#endif
#ifdef with_ExtraBasics
	//	template<class TNumeric> string dbgstr_extraBasic(const TNumeric src);
#endif
	//	void addItem_string(char const* src);

	void addPrmstack(stdat_t stdat, void const* prmstack);
	void addParameter(char const* name, stdat_t stdat, stprm_t stprm, void const* member);

	void addSysvar(SysvarId id);

	void addCall(stdat_t stdat, void const* prmstk);
	void addCall(char const* name, stdat_t stdat, void const* prmstk);
	void addResult(char const* name, PDAT const* ptr, vartype_t type);
	
public:
	CStructedStrWriter& getWriter() const { return *writer_; }

private:
//	CVardataStrWriter(CVardataStrWriter const&) = delete;
};

// バッファを所有するバージョン
template<typename TBuf>
class CVardataStrWriterWithBuf
	: public CVardataStrWriter
{
public:
	CVardataStrWriterWithBuf()
		: CVardataStrWriter<TBuf>(&mybuf_)
	{ }
	string const& get() const { return mybuf_; }
	string&& get() { return std::move(mybuf_); }
private:
	string mybuf_;
};

// バッファを所有するバージョン
//using CVardataStrWriterTreeformed = CVardataStrWriterWithBuf<CTreeformedWriter>;
//using CVardataStrWriterLineformed = CVardataStrWriterWithBuf<CLineformedWriter>;

//------------------------------------------------
// 置く場所に困る系関数群
//------------------------------------------------

// 文字列リテラルの形式の文字列
extern string toStringLiteralFormat(char const* src);

// モジュールクラス名の文字列の生成
extern string makeModuleClassNameString(stdat_t stdat, bool bClone);

// 配列添字の文字列の生成
extern string makeArrayIndexString(size_t dim, int const indexes[]);

// aptr から添字を求める
extern void calcIndexesFromAptr(int* indexes, APTR aptr, int const* lengths, size_t cntElems, size_t dim);

// スコープ解決を取り除いた識別子
extern string removeScopeResolution(string const& name);

// 仮引数の名前 (failure: "(idx)")
extern string getStPrmName(stprm_t stprm, int idx);

#endif
