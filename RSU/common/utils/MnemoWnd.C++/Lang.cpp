#include "Queue.h"
#include "Lang.h"
#include "QueueAPM.h"
#include "Function.h"
#include "socketclient.h"
#include "Api/MnemoSession.h"

bool CLang::ms_bAddQueue = true;
int  CLang::ms_nStack = 0;
char CLang::szCommonParam[4][256];

CLang::CLang(SQueue& queue)
	: m_queue(queue)
	, lgPipe(0x000000)// Цвет края
	, fnPipe(0xffffff)// Цвет центра
	, dkPipe(0x000000)// Цвет края
	, bkColor(0xc0c0c0)// Цвет фона
	, nCurrReg(-1)
	, penCount(0)
	, fntCount(0)
	, brsCount(0)
	, dwPar(0)
	, szPtr(NULL)
	, szBeg(NULL)
	, szEnd(NULL)
	, m_dwShift(0)
	, nIf(0), kIf(0)
{
	m_szStr = new char[0x1000];
	m_szStr[0] = 0;
	bIf[0] = true;
}

CLang::~CLang()
{
	if (m_szStr)
		delete[] m_szStr;
}

void CLang::NextTeg()
{
	dwPar = 0;
	*szComm = '\0';
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*inline*/ int CLang::AddKind(UINT type, const char* pszName, LONG_PTR* pnRegul)
{
	if (pnRegul) *pnRegul = nCurrReg;


	if (*szComm)
		return m_queue.m_pSocket->FindObj(type, szComm, pszName);
	return m_queue.m_pSocket->FindObj(type, pszName, pszName);
}

/*inline*/ int CLang::AddKindEx(UINT type, const char* name, LONG_PTR* pnRegul)
{
	//
	int nNumber = -1;
	if (lstrcmp(name, ".") == 0 || lstrcmp(name, "*") == 0)
	{
		nNumber = GetActiveObjSocket();
		if (pnRegul) *pnRegul = nCurrReg;
	}
	else
		nNumber = AddKind(type, name, pnRegul);
	//
	return nNumber;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*inline*/ int CLang::AddKindPipe(UINT type, const char* pszName, LONG_PTR* pnRegul)
{
	if (pnRegul) *pnRegul = nCurrReg;
	//
	if (*szComm)
		return m_queue.m_pPipe->FindObj(type, szComm, pszName);
	return m_queue.m_pPipe->FindObj(type, pszName, pszName);
}

/*inline*/ int CLang::AddKindExPipe(UINT type, const char* name, LONG_PTR* pnRegul)
{
	//
	int nNumber = -1;
	if (lstrcmp(name, ".") == 0 || lstrcmp(name, "*") == 0)
	{
		nNumber = GetActiveObj();
		if (pnRegul) *pnRegul = nCurrReg;
	}
	else
		nNumber = AddKindPipe(type, name, pnRegul);
	//
	return nNumber;
}

//void CLang::AddPribor(HPRIBOR hPribor)
//  {
//  if ( (m_queue.cntPribor % 10) == 0 )
//    {
//    int maxPribor =  m_queue.maxPribor+10;
//    HPRIBOR* arrPribor = (HPRIBOR*)::BuffAlloc(maxPribor*sizeof(HPRIBOR),true);
//    //
//    if ( arrPribor != NULL )
//      {
//      if (  m_queue.arrPribor != NULL )
//      memcpy( arrPribor, m_queue.arrPribor, m_queue.cntPribor*sizeof(HPRIBOR) );
//      ::BuffFree(m_queue.arrPribor);
//      m_queue.arrPribor = arrPribor;
//      m_queue.maxPribor = maxPribor;
//      }
//    else
//      {
//      // Это очень плохо.
//      ::BuffFree(m_queue.arrPribor);
//      m_queue.arrPribor = NULL;
//      m_queue.maxPribor = 0;
//      m_queue.cntPribor = 0;
//      }
//    }
//  //
//  if ( m_queue.arrPribor != NULL )
//    {
//    m_queue.arrPribor[m_queue.cntPribor++] = hPribor;
//    }
//  //
//  }

//int CLang::AddPribor(UINT type,char* name,LPSPROLOZ p,HPRIBOR hPribor,LONG_PTR* pnRegul)
//  {
//  AddPribor(hPribor);
//  p->nSocketNumber = AddKindEx(type,name,pnRegul);
//  return p->nSocketNumber;
//  }
//
//int CLang::AddPriborPipe(UINT type,char* name,LPSPROLOZ p,HPRIBOR hPribor,LONG_PTR* pnRegul)
//  {
//  AddPribor(hPribor);
//  p->nNumber = AddKindExPipe(type,name,pnRegul);
//  return p->nNumber;
//  }
//int CLang::GetCountsObj()
//  {
//  return (int)m_queue.m_pPipe->Count();
//  }
int CLang::GetActiveObj()
{
	return (int)m_queue.m_pPipe->Active();
}

int CLang::GetActiveObjSocket()
{
	if (m_queue.m_pSocket != NULL)
		return (int)m_queue.m_pSocket->Active();
	else
		return -1;
}
UINT CLang::GetMnemoType(int nNumber)
{
	return m_queue.m_pSocket->GetMnemoType(nNumber);
}
int CLang::GetMnemoTags(int nNumber)
{
	return m_queue.m_pSocket->GetMnemoTags(nNumber);
}
const char* CLang::GetModelName(int nNumber)
{
	return m_queue.m_pSocket->GetModelName(nNumber);
}

const char* CLang::GetModelNamePipe(int nNumber)
{
	return m_queue.m_pPipe->GetModelName(nNumber);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Динамический заказ памяти при построении очереди.
//
LPSPROLOG CLang::MemoryEx(SBuffer& mem, unsigned long iDelta)
{
	if (!Memory(mem, iDelta)) return NULL;
	LPSPROLOG p = (LPSPROLOG)(mem.szQueue + mem.nPtr);
	p->btType = TYPE_LIBRARY;
	p->wdSizeOf = (WORD)iDelta;
	mem.nPtr += iDelta;
	p->dwShift = (DWORD)(szBeg - szPtr);
	p->dwSizeT = (DWORD)(szEnd - szBeg);
	if (ms_nStack) p->dwSizeT = 0;
	return p;
}
//
bool CLang::Memory(SBuffer& mem, unsigned long iDelta)
{
	unsigned long nPtr = mem.nPtr + iDelta + sizeof(Queue_Null);
	if (nPtr >= mem.dwQueueMax)
	{
		unsigned long oldSize = mem.dwQueueMax;
		mem.dwQueueMax *= 2;
		mem.dwQueueMax += 0x80000;
		//
		void* str = BuffAlloc(mem.dwQueueMax, true);
		if (mem.szQueue != NULL)
		{
			memcpy(str, mem.szQueue, oldSize);
			BuffFree(mem.szQueue);
		}
		mem.szQueue = (LPBYTE)str;
		if (mem.szQueue != NULL)
			mem.szQueue[mem.nPtr + iDelta] = TYPE_NULL;
	}
	return (mem.szQueue != NULL);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//unsigned char SkipQueue(unsigned char*& szQueue)
//  {
//  LPSPROLOG p = (LPSPROLOG)szQueue;
//  if ( p->btType != TYPE_NULL )
//    szQueue += p->wdSizeOf;
//  return p->btType;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace managed
{
	void RegisterElement(const char* tag, void* data, void* priborData/* = nullptr*/)
	{
		auto session = (mnemownd::api::MnemoSession*)CurrentMnemoSession;
		ParsingCallbacks.ClearSharedData();
		for (int i = 0; i < session->GetPipeClient()->Count(); ++i)
		{
			ParsingCallbacks.AppendPipeData((void*)session->GetPipeClient()->Sh(i));
		}

		for (int i = 0; i < session->GetSocketClient()->Count(); ++i)
		{
			ParsingCallbacks.AppendSocketData((void*)session->GetSocketClient()->Sh(i));
		}

		ParsingCallbacks.RegisterElement(tag, data, &session->GetQueue(), priborData);
	}
}
