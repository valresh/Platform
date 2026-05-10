#include "QuickBuilder.h"
#include <RsuX.h>
#include <macros/StrHelps.h>
#include <QB_Class.h>
#include <rsuErr.h>
#include <crosslock.h>

using namespace nRSUx;

KQuickBuilder* g_pQuickBuilder = NULL;
static std::recursive_mutex s_CS;

int CompQb(const void* pP1, const void* pP2)
{
	KQbBase* pI1 = *(KQbBase**)pP1;
	KQbBase* pI2 = *(KQbBase**)pP2;
	return strcmp(pI1->PointName, pI2->PointName);
}

KQbBase* KQuickBuilder::Find(LPCSTR pszQb)
{
	LOCK(s_CS);
	//KCsSync _sync( m_csOnContour );

	static SQB_STATUS Key;
	Key.PointName = pszQb;
	KQbBase* p = &Key;
	KQbBase** pI = (KQbBase**)bsearch(&p, m_ppPoints, m_nPoints, sizeof(KQbBase*), CompQb);
	if (!pI)
	{
		for (DWORD n = 0; n < m_nPoints; ++n)
		{
			if (!strcmp(m_ppPoints[n]->PointName, pszQb))
				return m_ppPoints[n];
		}
		DWORD types[] = { W_QBANALOG::TypeID, W_QBSTATUS::TypeID };
		for (DWORD i = 0; i < _countof(types); ++i)
		{
			LPCSTR pszEntry = NULL;
			CBase* pSrc = KMemoryServiceQB5xx::Instance().FindIOsStruct(pszQb, types[i], 0, &pszEntry);
			if (pSrc)
			{
				if (!strcmp(pszQb, pszEntry))
					return NULL;
				return Find(pszEntry);
			}
		}
		return NULL;
	}
	return *pI;
}

SParamInfo RsuConnectParamInfo(LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix)
{
	SParamInfo info;
	if (!g_pQuickBuilder)
		return info;
	char buf[4][32 * 4];
	int indexField = 1;
	SplitString(pszPointName, buf, '.', false);

	KQbBase* pObj = g_pQuickBuilder->Find(buf[0]);
	if (!pObj)
	{
		sprintf_s(buf[3], "%s.%s", buf[0], buf[1]);
		pObj = g_pQuickBuilder->Find(buf[3]);
		if (!pObj)
			return info;
		indexField = 2;
	}
	BYTE* pMem = NULL;
	eVarType type = evtHZ;
	if (!pObj->GetVar(buf[indexField], &pMem, &type))
		return info;
	switch (type)
	{
	default:
		ASSD(0);
		break;
	case evtInt2:
		info << (short*)pMem;
		break;
	case evtDouble:
		info << (double*)pMem;
		break;
	}
	return info;
}
