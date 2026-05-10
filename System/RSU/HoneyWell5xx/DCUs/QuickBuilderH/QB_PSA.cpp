#include "QB_Class.h"
#include <rsuErr.h>
#include "QBPARM_INIT.h"
#include "ParmVarInfo.h"
#include <_alarmentry.h>
#include <ConnectQB.h>

LIST_PARM(SQB_PSA, W_QBPSA, 10)

void SQB_PSA::InitParm()
{
#include <QuickBuilder/Psa.h>
	s_defFlag = SVarInfo::efParam;
#include <QuickBuilder/Psa_P.h>
	qsort(VarInfo, kVarInfo, sizeof(SVarInfo), CompVarInfo);
}

void SQB_PSA::GetAsParams(KQbBridge2SysParam& paramCont)
{
	if (!W)
		return;
	W->GetParams(paramCont);
	KQbBase::GetAsParams(paramCont);
}

LPCSTR SQB_PSA::GetTypeName()
{
	return W->ClassName;
}


void SQB_PSA::StepT(double dt)
{
	KQbBase::StepT(dt);
}

void SQB_PSA::Link()
{
	KQbBase::Link();
	ScriptsAfterStateRestored();
}

SUserDef* SQB_PSA::GetUserDefs(DWORD& nCount)
{
	nCount = _countof(W->userDefined);
	return W->userDefined;
}
