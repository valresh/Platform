#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_DAlrmGen_P
{
#include "PrivateVars/DigAlrm.h"
};


class K_DAlrmGen
	: public KFlexBlockBase
	, protected S_DAlrmGen_P
{
	KFlexConnection** m_ppIn;
	DWORD m_nIns;
	S_DAlrmGen* W;
	SFlex* pRaW;
public:
	K_DAlrmGen()
	{
		m_nullOrderExec = exnOutputsIO;
	}
	static KFlexBlockBase* Create();
protected:
	virtual void OnCreated();
	virtual void AssignConnections(KFlexConnection** ppIns, DWORD nIns, KFlexConnection** ppOuts, DWORD nOuts);
	SReturn StepCalc(SCallParams& params);
	virtual void OnAfterRestoreState();
	static void InitParm();
};

static KBlockCreator _DAlrmGen("DALRMGEN", K_DAlrmGen::Create);

KFlexBlockBase* K_DAlrmGen::Create()
{
	return new K_DAlrmGen;
}

static LPCSTR s_pszON = "$Red.ON";
static LPCSTR s_pszOFF = "$Blue.OFF";

namespace ns_DAlrmGen
{
#include <_InitInfo.h>
	DECLARE_PRIVATE_VARSINFO(K_DAlrmGen, 10)
	DECLARE_SHARED_VARSINFO(S_DAlrmGen, 20)
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_DAlrmGen;
void K_DAlrmGen::InitParm()
{
#include <FlexBlocks/DAlrmGen.h>
	qsort(s_sharedVarInfo, s_ksharedVarInfo, sizeof(SVarInfo), CompVarInfo);
	//#include "PrivateVars/DAlrmGen.h"
#include "PrivateVars/DigAlrm.h"
	qsort(s_privateVarInfo, s_kprivateVarInfo, sizeof(SVarInfo), CompVarInfo);
}

void K_DAlrmGen::OnCreated()
{
	pRaW = CreateSFlexSharedObj(m_szVisibleName, &W);

	if (!s_kprivateVarInfo)
		InitParm();

	m_kPrivateVarsInfo = s_kprivateVarInfo;
	m_pPrivateVarsInfo = s_privateVarInfo;
	m_pPrivateObject = (BYTE*)this;

	m_kSharedVarsInfo = s_ksharedVarInfo;
	m_pSharedVarsInfo = s_sharedVarInfo;
	m_pSharedObject = (BYTE*)W;
}

void K_DAlrmGen::AssignConnections(KFlexConnection** ppIns, DWORD nIns, KFlexConnection** ppOuts, DWORD nOuts)
{
	m_ppIn = CloneConnections(m_nIns, ppIns, nIns);
	for (DWORD i = 0; i < nIns; ++i)
	{
		KFlexConnection* pIn = m_ppIn[i];
		if (strcmp(pIn->m_szSink, "IN") == 0)
			(*pIn) >> PV;
		else if (strcmp(pIn->m_szSink, "P1") == 0)
			(*pIn) >> W->P1;
		else if (strcmp(pIn->m_szSink, "P2") == 0)
			(*pIn) >> W->P2;
	}
}

template<size_t _S>
void FltToStr(char(&szBuf)[_S], tAnalog32 d)
{
	double ab = fabs(d);
	if (ab < 0.001 || ab > 100000)
	{
		if (d == 0)
			sprintf_s(szBuf, "0");
		else
			sprintf_s(szBuf, "%.1e", d);
	}
	else
	{
		//
		char szFrm[8] = "%.5f";
		sprintf_s(szBuf, szFrm, d);
		char* pStr = strchr(szBuf, '.');
		if (pStr == NULL) //Русифицированный формат
			pStr = strchr(szBuf, ',');
		if (pStr)
		{
			*pStr = '.';//Чтобы избавиться от запятых
			char* pTmp = szBuf + strlen(szBuf) - 1;
			while (pTmp != pStr && *pTmp == '0')
				*(pTmp--) = '\0';
			if (pTmp == pStr)
				*pTmp = '\0';
		}
	}
}

template<size_t _S>
void GetText(char(&szBuf)[_S], tAnalog32 ap)
{
	sprintf_s(szBuf, "%.2f", ap);
}

template<size_t _S>
void StrPX(char(&str)[_S], LPCSTR P, LPCSTR val)
{
	LPSTR btr = strstr(str, P);
	if (btr == NULL)
		return;
	*btr = 0;
	char* etr = btr + strlen(P);

	char out[_S] = {};
	sprintf_s(out, "%s%s%s", str, val, etr);
	strcpy_s(str, out);
}

void K_DAlrmGen::OnAfterRestoreState()
{
	W->STATE = 0;
	W->SetAck = 0;
	W->Acked = 1;
	PV = PREVPV = 0;
}

KFlexBlockBase::SReturn K_DAlrmGen::StepCalc(SCallParams& params)
{
	SReturn sr;
	tDigital32 PREVPV = PV;
	tAnalog32* aps[] = { &W->P1, &W->P2 };
	for (DWORD i = 0; i < m_nIns; ++i)
	{
		KFlexConnection* pIn = m_ppIn[i];
		switch (pIn->m_szSink[1])
		{
		case '1':
		case '2':
			break;
		default:
			continue;
		}
		int index = pIn->m_szSink[1] - '1';
		(*pIn) >> *aps[index];
	}

	for (DWORD i = 0; i < m_nIns; ++i)
	{
		KFlexConnection* pIn = m_ppIn[i];
		pIn->GetData();
	}
	if (!PREVPV && PV)
	{
		W->STATE = PV;
		char param1[64] = {}, param2[64] = {};
		GetText(param1, *aps[0]);
		GetText(param2, *aps[1]);

		char str[_countof(TEXT) * 2] = {};
		strcpy_s(str, TEXT);
		StrPX(str, "%P1", param1);
		StrPX(str, "%P2", param2);
		if (PV /*!= PREVPV*/)
		{
			if (pFlexAlarm)
			{
				char szAtValue[32] = {};
				snprintf(szAtValue, _countof(szAtValue), "%d", PV);
				if (W->STATE)
				{
					W->SetAck = 0;
					W->Acked = 0;
					// pFlexAlarm( pRaW, true, s_pszON, params.modelTime, params.pszDrName, W->DESCR, W->DESCR1, W->DESCR2, "", W->Acked, "", "", "" );
					pFlexAlarm(pRaW, true, str, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", W->AlarmVersion, W->AlarmColor); // Taken from merge
				}
				else
				{
					pFlexAlarm(pRaW, false, str, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", W->AlarmVersion, W->AlarmColor);
					W->STATE = 0;
				}
				// pFlexAlarm( pRaW, false, s_pszOFF, params.modelTime, params.pszDrName, W->DESCR, W->DESCR1, W->DESCR2, "", W->Acked, "", "", "" );
			  // if( W->STATE && W->SetAck )
			  // {
				// W->ConfirmAlarm = 1;
				// W->SetAck = 0;
			  // }
			  // else
				// W->ConfirmAlarm = 0;
			}
		}
	}
	if (W->SetAck)
	{
		if (!W->Acked && pFlexAckingAlarm)
			pFlexAckingAlarm(pRaW, params.modelTime, W->STATE ? true : false);
		W->Acked = 1;
		W->SetAck = 0;
	}
	//PREVPV = PV;
	return sr;
}