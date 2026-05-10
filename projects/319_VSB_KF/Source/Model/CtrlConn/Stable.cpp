#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include "CommProc.h"


CStableConnect * CStableConnect::pFirst = NULL;
CStableConnect * CStableConnect::pLast = NULL;
bool CStableConnect::UseStable = false; // использовать
bool CStableConnect::Record = false;    // записывать
bool CStableConnect::Evaluate = false;  // Оценивать
int CStableConnect::kPnt = 0;
CStableCtrl * CStableConnect::pCtrlData = NULL;
int CStableConnect::CallControl = -1;

CStableConnect::CStableConnect( CConnect * _pConn )
  {
  memset ( this, 0, sizeof ( *this ));
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  pConn = _pConn;
//
  }

void CStableConnect::Work( )
  {
  if ( !Record || !UseStable )
    return;
  if ( pData == NULL )
    return;
  if ( Type == 'D' )
    {
    int I;
    if ( !pData->GetValue( 'D', &I ))
      return;
    if ( I != State && kPnt > 0 )
      Change++;
    State = I;
    return;
    }
  double A;
  if ( !pData->GetValue( 'A', &A ))
    return;
  if ( Evaluate )
    {
    double M = Summ / kPnt;
    double D = fabs ( M - A );
    if ( Deviation < D )
      Deviation = D;
    }
  Summ += A;
  }

struct CStableCtrl 
  {
  int StepStart;
  int StepEvaluate;
  int StepFinish;
  bool WriteErrDiscrete;
  bool WriteErrAnalog;
  double CritMed;  // Ниже этой величины не оцениваем
  double CritProc; // Критический поцент отклонений
//
  int kErrAnalog;
  int kErrDiscrete;
  };

bool CStableConnect::Init( )
  {
  // HANDLE hVarMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"Тест стабильности");
  // if ( hVarMapping == NULL )
  //   {
  //   UseStable = false; // использовать
  //   Record = false;    // записывать
  //   Evaluate = false;  // Оценивать
  //   CallControl = -1;
  //   return false;
  //   }
  // pCtrlData = (CStableCtrl *)MapViewOfFileEx( hVarMapping, FILE_MAP_ALL_ACCESS,
  //   0,
  //   0,
  //   0,
  //   NULL );
  //if ( pCtrlData == NULL )
  return false;
  UseStable = true; // использовать
  CallControl = 0;
  pCtrlData->kErrAnalog = 0;
  pCtrlData->kErrDiscrete = 0;
  return true;
  }


static int kStep;
int CStableConnect::Control( )
  {
  if ( pCtrlData == NULL )
    {
    CallControl = -1;
    return -1;
    }
  kStep++;
  if ( kStep < pCtrlData->StepStart )
    {
    CallControl = 0;
    return 0;
    }
  Record = true;
  if ( kStep >= pCtrlData->StepEvaluate )
    Evaluate = true;
  CStableConnect * pSt = pFirst;
  while ( pSt )
    {
    pSt->Work();
    pSt = pSt->pNext;
    }
  kPnt++;
  CallControl = 1;
  if ( kStep < pCtrlData->StepFinish )
    return 1;
//
  double CritMed = pCtrlData->CritMed;  // Ниже этой величины не оцениваем
  double CritDelta = pCtrlData->CritProc * 0.01; // Критический поцент отклонений
  void * F = NULL;
  if ( pCtrlData->WriteErrAnalog || pCtrlData->WriteErrDiscrete )
    {
    char Path[1024];
    sprintf_s ( Path, 1024, "%sINI\\Stable.txt",  PROJECT_ROOT );
    F = Fopen ( Path, "wt" );
    }
//
  int kErrAnalog = 0;
  int kErrDiscrete = 0;
  pSt = pFirst;
  while ( pSt )
    {
    if ( pSt->Type == 'A' )
      {
      double M = fabs ( pSt->Summ / kPnt );
      if ( M > CritMed )
        {
        double D = M * CritDelta;
        if ( pSt->Deviation > D )
          {
          kErrAnalog++;
          if ( pCtrlData->WriteErrAnalog )
            {
            Fprintf  ( F, "%s->%s(%s) средн = %4.1lf, откл = %5.2lf,\n", (char*)pSt->pConn->RefFrom, (char*)pSt->pConn->RefTo, (char*)pSt->pConn->Desc, 
                        M, pSt->Deviation  );
            }
          }
        }
      }
    pSt = pSt->pNext;
    }
  pSt = pFirst;
  while ( pSt )
    {
    if ( pSt->Type == 'D' )
      {
      if ( pSt->Change )
        {
        kErrDiscrete++;
        if ( pCtrlData->WriteErrDiscrete )
          {
          Fprintf  ( F, "%s->%s(%s) %d изменений\n", (char*)pSt->pConn->RefFrom, (char*)pSt->pConn->RefTo, (char*)pSt->pConn->Desc, pSt->Change );
          }
        }
      }
    pSt = pSt->pNext;
    }
//
  if ( F )
    Fclose ( F );
  pCtrlData->kErrAnalog = kErrAnalog;
  pCtrlData->kErrDiscrete = kErrDiscrete;
  CallControl = -1;
  //UnmapViewOfFile( pCtrlData );
  return -1;
  }

