#ifndef IG_CLASS_FUNCTION_CALLER_H
#define IG_CLASS_FUNCTION_CALLER_H

// 関数呼び出しのオブジェクト
// 引数から直接 prmstack の生成を行う。
// CCaller, CCall の代わりにするつもり。

// CStreamCaller に似ているが、呼び出し時に追加の引数を与えられない。

#include "hsp3plugin_custom.h"
#include "Functor.h"
#include "CPrmStkCreator.h"

class CFuncCaller;

using funcCaller_t = CFuncCaller*;

class CFuncCaller
	: public IFunctor
{
	// メンバ変数
private:
	functor_t functor_;
	CPrmInfo& prminfo_;
	CPrmStkCreatorWithBuffer prmstk_;

	// 構築
private:
	CFuncCaller() = delete;
	CFuncCaller(functor_t f)
		: functor_ { std::move(f) }
		, prminfo_ { f->getPrmInfo() }
		, prmstk_(f->getPrmInfo().getStackSize() + sizeof(void*))
	{ }
	~CFuncCaller() { }

	CFuncCaller(CFuncCaller const&) = delete;

public:
	void call(CCaller& caller) {
		functor_->call();
		return;
	}

	label_t   getLabel() const override { return functor_.getLabel(); }
	int       getAxCmd() const override { return functor_.getAxCmd(); }
	int       getUsing() const override { return functor_.getUsing(); }

	CPrmInfo const& getPrmInfo() const override { return prminfo_; }

	// ラッパー
	static funcCaller_t New(functor_t f) { return new CFuncCaller(f); }
};

#endif
