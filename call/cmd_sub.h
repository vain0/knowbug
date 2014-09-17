// Call - SubCommand header

#ifndef IG_CALL_SUB_COMMAND_H
#define IG_CALL_SUB_COMMAND_H

#include "hsp3plugin_custom.h"
#include "mod_argGetter.h"
#include "mod_makepval.h"
#include "axcmd.h"

#include "CCall.h"
#include "Functor.h"
#include "cmd_call.h"

#include "CPrmInfo.h"

//################################################
//    Call 下請け関数
//################################################
// 仮引数リスト関連
extern void DeclarePrmInfo(label_t lb, CPrmInfo&& prminfo);
extern CPrmInfo const& GetPrmInfo(label_t);
extern CPrmInfo const& GetPrmInfo(stdat_t);

extern int code_get_prmtype(int deftype = PRM_TYPE_NONE);
extern CPrmInfo::prmlist_t code_get_prmlist();
extern CPrmInfo code_get_prminfo();

// その他
extern int GetPrmType( char const* s );

extern void* GetReferedPrmstk( stprm_t pStPrm);

//################################################
//    下請け関数
//################################################
template<class T>
bool numrg(T const& val, T const& min, T const& max)
{
	return (min <= val && val <= max);
}

// 引数取得
extern functor_t code_get_functor();

#endif
