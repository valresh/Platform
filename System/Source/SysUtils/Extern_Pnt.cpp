#include "Extern_Pnt.h"
#include "BaseModel.h"
#include "Connect.h"
#include "Param.h"

void ACS_Analog(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	while(pPnt)
	{
		if(pPnt->Type[0] == 'A') pPnt->GetParams();
        pPnt = pPnt->pNext_Model_Pnt;
	}
}

void All_Connected(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	while(pPnt)
	{
		if(pPnt->Type[1] == 'I' && !pPnt->IsConnection)
		{
			pPnt->Connection = "Auto";
			pPnt->IsConnection = true;
		}
        pPnt = pPnt->pNext_Model_Pnt;
	}
}

void ACS_Diskrete(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	while ( pPnt )
	{
		if ( pPnt->Type[0] == 'D' )
			pPnt->GetParams( );
        pPnt = pPnt->pNext_Model_Pnt;
	}
}

void ACS_Connect( IBaseModel * pModel )
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	while ( pPnt )
	{
		if ( pPnt->Type[1] == 'I' && !pPnt->IsConnection)
		{
			pPnt->IsConnection = true;
			pPnt->Connection == "#";
		}
        pPnt = pPnt->pNext_Model_Pnt;
	}
}

void ACS_Params( char * TabName, IBaseModel * pModel )
{
    if ( pModel == NULL || QParams::ReadProps )
		return;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	int K = 0;
	while ( pPnt )
	{
		if ( pPnt->Type[0] == 'A' )
			K++;
        pPnt = pPnt->pNext_Model_Pnt;
	}
	bool TotalTab = false;
	if ( K > 0 )
	{
        if ( TabName[0] && !QParams::SaveAllParams && !QParams::ReadProps )
		{
			TotalTab = true;
			static int Show1 = 0;
            QParams & __P1 = QParams::Get();
			__P1.SetDefaultValue(NULL);
			__P1.Set( pModel, $ TabName, Show1, 4 );
			__P1.Type = 'T';
			__P1.Class = 'X';
			static int Show2 = 0;
            QParams & __P2 = QParams::Get();
			__P2.SetDefaultValue(NULL);
			__P2.Set( pModel, $"Аналоговые точки", Show2, 4 );
			__P2.Type = 'T';
			__P2.Class = 'X';
		}
        pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
		while ( pPnt )
		{
			if ( pPnt->Type[0] == 'A' )
				pPnt->GetParams( );
            pPnt = pPnt->pNext_Model_Pnt;
		}
		static int Show3 = 0;
        QParams & __P3 = QParams::Get();
		__P3.SetDefaultValue(NULL);
		__P3.Set( pModel, $"Аналоговые точки", Show3, 4 );
		__P3.Type = 'E';
		__P3.Class = 'X';
	}
	K = 0;
    pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
	while ( pPnt )
	{
		if ( pPnt->Type[0] == 'D' )
			K++;
        pPnt = pPnt->pNext_Model_Pnt;
	}
	if ( K > 0 )
	{
        if ( TabName[0] && !QParams::SaveAllParams && !QParams::ReadProps )
		{
			if ( !TotalTab )
			{
				static int Show1 = 0;
                QParams & __P1 = QParams::Get();
				__P1.SetDefaultValue(NULL);
				__P1.Set( pModel, $ TabName, Show1, 4 );
				__P1.Type = 'T';
				__P1.Class = 'X';
			}
			static int Show3 = 0;
            QParams & __P3 = QParams::Get();
			__P3.SetDefaultValue(NULL);
			__P3.Set( pModel, $"Дискретные точки", Show3, 4 );
			__P3.Type = 'T';
			__P3.Class = 'X';
		}
        pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
		while ( pPnt )
		{
			if ( pPnt->Type[0] == 'D' )
				pPnt->GetParams( );
            pPnt = pPnt->pNext_Model_Pnt;
		}
		static int Show4 = 0;
        QParams & __P4 = QParams::Get();
		__P4.SetDefaultValue(NULL);
		__P4.Set( pModel, $"Дискретные точки", Show4, 4 );
		__P4.Type = 'E';
		__P4.Class = 'X';
	}
	if ( TotalTab )
	{
		static int Show5 = 0;
        QParams & __P5 = QParams::Get();
		__P5.SetDefaultValue(NULL);
		__P5.Set( pModel, $ TabName, Show5, 4 );
		__P5.Type = 'E';
		__P5.Class = 'X';
	}
}

void ACS_Params( IBaseModel * pModel )
{
	ACS_Params( "АСУ", pModel );
}

int CompSave(const void * p1, const void * p2)
{
	CSavePnt * s1 = (CSavePnt*)p1;
	CSavePnt * s2 = (CSavePnt*)p2;
	return lstrcmp ( s1->Name, s2->Name );
}

CSavePnt* CExtern_Pnt::pSave;
int CExtern_Pnt::kPnt;

CExtern_Pnt::CExtern_Pnt(struct IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr )
  {
  if(pFirst_Extern_Pnt == NULL) pFirst_Extern_Pnt = this;
  if(pLast_Extern_Pnt) pLast_Extern_Pnt->pNext_Extern_Pnt = this;
  pLast_Extern_Pnt = this;
  pNext_Extern_Pnt = NULL;
  pNext_Model_Pnt = _pModel->pModel_Pnt;
  _pModel->pModel_Pnt = this;
    //
	Point = $ _Point;
	int n = 0;
	while ( n < 3 )
	{
		Type[n] = _Type[n];
		if(_Type[n] == 0) break;
		n++;
	}
	Type[n] = 0;
	pModel = _pModel;
	nDefect = 0;
	Flag = 0;
	Descr = $ _Descr;
	pModel->AddPoint(this);
	SD = ' ';
}

CExtern_Pnt::CExtern_Pnt()
{
	pModel = NULL;
}


void CExtern_Pnt::SaveAll()
{
	CExtern_Pnt * pPnt = pFirst_Extern_Pnt;
	int kPnt = 0;
	while(pPnt)
	{
		if(!pPnt->IsConnection)
		{
			pPnt = pPnt->pNext_Extern_Pnt;
			continue;
		}
		kPnt++;
		pPnt = pPnt->pNext_Extern_Pnt;
	}
	//
	ASS(pSRFile->WF(&kPnt, sizeof(kPnt)));
	if(kPnt > 0)
	{
		CSavePnt * pSave = new CSavePnt[kPnt];
		int nPnt = 0;
		CExtern_Pnt * pPnt = pFirst_Extern_Pnt;
		while(pPnt)
		{
			if(!pPnt->IsConnection)
			{
				pPnt = pPnt->pNext_Extern_Pnt;
				continue;
			}
            sprintf(pSave[nPnt].Name, "%s.%s", pPnt->pModel->ObjName.Str, pPnt->Point.Str);
			//Я
			void * address = pPnt->GetAddress();
			char ch = pPnt->Type[0];
			if(ch == 'A') pSave[nPnt].Int = *(int*)address;
			if(ch == 'D') pSave[nPnt].Dbl = *(double*)address;
			nPnt++;
			pPnt = pPnt->pNext_Extern_Pnt;
			continue;
			/*
			if(pPnt->Type[0] == 'A')
			{
				CAnalog * pA = (CAnalog*)pPnt;
				pSave[nPnt].Dbl = pA->Value;
				nPnt++;
				pPnt = pPnt->pNext_Extern_Pnt;
				continue;
			}
			if(pPnt->Type[0] == 'D')
			{
				CDiscrete * pD = (CDiscrete*)pPnt;
				pSave[nPnt].Int = pD->Value;
				nPnt++;
				pPnt = pPnt->pNext_Extern_Pnt;
				continue;
			}*/
			ASS(0)
				pPnt = pPnt->pNext_Extern_Pnt;
		}
		ASS(pSRFile->WF(pSave, kPnt * sizeof(CSavePnt)));
		delete[] pSave;
	}
}

void CExtern_Pnt::RestoreAll()
{
	ASS(pSRFile->RF(&kPnt, sizeof(kPnt)));
	if(kPnt <= 0) return;
	pSave = new CSavePnt[kPnt];
	ASS(pSRFile->RF( pSave, kPnt * sizeof(CSavePnt)));
	if(kPnt >= 2)
		qsort( pSave, kPnt, sizeof(CSavePnt), CompSave);
}



void CExtern_Pnt::Init(IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr)
{
	Point = _Point;
	int n = 0;
	while(n < 3)
	{
		Type[n] = _Type[n];
		if(_Type[n] == 0) break;
		n++;
	}
	Type[n] = 0;
	pModel = _pModel;
	nDefect = 0;
	Descr = _Descr;
	if(pModel)
		pModel->AddPoint(this);
}

bool CExtern_Pnt::CompTypes( char * EXT_Type )
{
	if ( EXT_Type[0] != Type[0] )
		return false;
	if ( Type[1] == 'M' )
		return true;
	char T = EXT_Type[1];
	if ( T == 'I' && Type[1] != 'O' )
		return false;
	if ( T == 'O' && Type[1] != 'I' )
		return false;
	return true;
}
