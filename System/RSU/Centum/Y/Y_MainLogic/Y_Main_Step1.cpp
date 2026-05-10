#include "Y_MainLogic.h"
#include <macros/StrFormats.h>
#include <macros/AutoCloser.h>
#include <rsuProjectVersion.h>

using namespace ns_rsu;

int g_initSteps = 0;

void CY_MainLogic::InitPrevConnectionsSources()
{
  eVersion ver = rsuProjectVersion();
  for ( int n = 0; n < kConnection; n++ )
  {
    CObjConnection * pC = &Connection[n];
    pC->bTest4SrcChanged = false;
    if( GOBKK_MSK!=ver )
      continue;
    switch( pC->ConnectionType )
    {
    /*case pC->ectSignal2Signal:
    case pC->ectVar2Signal:*/
    case pC->ectSignalMove2Var:
    /*case pC->ectSignalConvert2Var:
    case pC->ectSignal2Action:
    case pC->ectSignalMove2Action:
    case pC->ectSignalConvert2Action:*/
      break;
    default:
      continue;
    }
    if( !pC->pOut )
      continue;
    switch( pC->TypeOut )
    {
    case 'D':
      pC->prevSrcValue.D = *(double*)(pC->pOut);
      break;
    case 'I':
      pC->prevSrcValue.I = *(int*)(pC->pOut);
      break;
    case 'B':
      pC->prevSrcValue.B = *(bool*)(pC->pOut);
      break;
    case 'W':
      pC->prevSrcValue.W = *(BYTE*)(pC->pOut);
      break;
    default:
      continue;
    }
    pC->bTest4SrcChanged = true;
  }
}

int CY_MainLogic::Step1L()
{
  for( int n = 0; n < kY_Obj; ++n )
  {
    pY_Obj[n]->ResetAlarms();
  }
  for ( int n = 0; n < kD_Obj; n++ )
  {
    pD_Obj[n]->ResetAlarms();
  }

  if( !(g_initSteps&1) )
  {
    InitPrevConnectionsSources();
    g_initSteps |= 1;
  }

  if( g_initSteps&2 )
    return 0;

  g_initSteps |= 2;
  const int kY_1 = kY_Obj - 1;
  for ( int n = 0; n < kY_Obj; ++n )
  {
    pY_Obj[n]->IsLimits = false;
    pY_Obj[n]->Prepare1();
    if( id_LC64==pY_Obj[n]->Y_Type || id_LC64_E==pY_Obj[n]->Y_Type )
      pY_Obj[n]->Level = 2;
    else
      pY_Obj[n]->Level = 1;

    if( n < kY_1 )
    {
      if( !strcmp(pY_Obj[n]->Name, pY_Obj[n+1]->Name) )
      {
        CAlarmBase::tFlagUse_ *pFl = pY_Obj[n]->GetFlagUse();
        CAlarmBase::tFlagUse_ *pFl2 = pY_Obj[n+1]->GetFlagUse();
        if( pFl )
          *pFl |= CAlarmBase::fuHaveSameName;
        if( pFl2 )
          *pFl2 |= CAlarmBase::fuHaveSameName;
      }
    }
  }

  {
    using namespace ns_strformats;
    char szMemoryUseResult[256];
    char szBufO[INT_SEP_STR_SIZE] = { 0 };
    char szBufN[INT_SEP_STR_SIZE] = { 0 };
    char szBufS[INT_SEP_STR_SIZE] = { 0 };
    int_sep( szBufO, KMemoryServiceY::Instance().centumObjInMemory );
    int_sep( szBufN, KMemoryServiceY::Instance().centumSharedDataInNames );
    int_sep( szBufS, KMemoryServiceY::Instance().centumStaticMemory );
    sprintf_s( szMemoryUseResult, "Использование пямити Centum'ум: objs %s, names %s, statics %s\n", szBufO, szBufN, szBufS );
    OutputDebugString( szMemoryUseResult );
    sprintf_s( szMemoryUseResult, "Создано Y_Obj=%d(предел %d), IOs=%d(предел %d), Con=%d(предел %d), DR=%d(предел %d)\n"
                                , kY_Obj, _countof(pY_Obj)
                                , kD_Obj, _countof(pD_Obj)
                                , kConnection, _countof(Connection)
                                , kDR, _countof(DR) );
    OutputDebugString( szMemoryUseResult );
  }

  int kRepeat = 1;
  int K = 0;
  while ( kRepeat && K < 5 )
    {
    K++;
    kRepeat = 0;
    for ( int n = 0; n < kY_Obj; n++ )
      {
      if ( !pY_Obj[n]->IsLimits )
        {
        bool Res = pY_Obj[n]->SetLimits();
        if ( Res )
          pY_Obj[n]->IsLimits = true;
        else
          kRepeat++;
        }
      }
    }

////////////////////////////////////////////////////////////////////////////////
  if( !m_bCreateYokoData )
  {
    return 0;
  }

  KAutoCloser<FILE*> F( fopen( m_szCsvFile, "wt" ), fclose );
  fprintf( F, "ED#Точка Y;ED#Alt;LS,TypeDV#Тип Y;ST#Описание;LE,Objects#Модель;LS,Type#Тип Модели;LE,Points#Точка_Модели;ED#Формула;ST#Примечание;\n" );

  int bNeedStationNumb = 0;
  int line = 2;
  for ( int n = 0; n < kD_Obj; ++n )
  {
    CY_DI * pY = pD_Obj[n];

    if( pY->controllerType!=CY_Base::ectRSU )
      continue;

    CY_Base *pObj = FormCsvRecord( F, pY, bNeedStationNumb, n, true, NULL, NULL, line );
    if( !pObj )
      continue;
    if( 'I'==pY->IOZ )
    {
      switch( pObj->Y_Type )
      {
      case id_SIO_22:
      case id_SIO_22E:
      case id_SIO_21:
      case id_SI_2:
        {
          CY_SIO_X *pSio = (CY_SIO_X*)pObj;
          if( !pSio->bNextIn )
            break;
          CY_DI *pNextDI = pY->GetNextDI( pY );
          if( pNextDI )
          {
            int delta = 0;
            for( delta; delta<kD_Obj; ++delta )
            {
              if( pNextDI==pD_Obj[delta] )
                break;
            }
            if( delta<kD_Obj )
            {
              char comment[128] = { 0 };
              sprintf_s( comment, "/Следующий дискрет %s", pObj->Model );
              FormCsvRecord( F, pNextDI, bNeedStationNumb, delta, true, pObj->Name, comment, line );
            }
          }
        }
        break;
      }
    }
    if( 'O'==pY->IOZ )
    {
      switch( pObj->Y_Type )
      {
      case id_ONOFF_G:
        {
          CY_DI *pNextDI = pY->GetNextDI( pY );
          if( pNextDI )
          {
            int delta = 0;
            for( delta; delta<kD_Obj; ++delta )
            {
              if( pNextDI==pD_Obj[delta] )
                break;
            }
            if( delta<kD_Obj )
            {
              char comment[128] = { 0 };
              sprintf_s( comment, "/Следующий дискрет %s", pObj->Model );
              FormCsvRecord( F, pNextDI, bNeedStationNumb, delta, true, pObj->Name, comment, line );
            }
          }
        }
        break;
      case id_SIO_22:
      case id_SIO_22E:
        {
          CY_SIO_X *pSio = (CY_SIO_X*)pObj;
          if( !pSio->bNextOut )
            break;
          CY_DI *pNextDI = pY->GetNextDI( pY );
          if( pNextDI )
          {
            int delta = 0;
            for( delta; delta<kD_Obj; ++delta )
            {
              if( pNextDI==pD_Obj[delta] )
                break;
            }
            if( delta<kD_Obj )
            {
              char comment[128] = { 0 };
              sprintf_s( comment, "/Следующий дискрет %s", pObj->Model );
              FormCsvRecord( F, pNextDI, bNeedStationNumb, delta, true, pObj->Name, comment, line );
              KKK();
            }
          }
        }
        break;
      }
    }
    
  }

  char Model[256] = {};
  char Point[256] = {};
  char cMes[64] = {};

  for ( int n = 0; n < kY_Obj; n++ )
  {
    CY_Base * pY = pY_Obj[n];
    if ( pY->Y_Type == id_FF_AI )
    {
      SetModel( pY->Name, CY_DI::eptAnalog, 'I', Model, Point, cMes, pY->pDR->NumbFCS, pY->pDR->domain );
      fprintf ( F, "%s;;AI;%s;%s;;%s;;%s;\n", pY->Name, pY->pComm, Model, Point, cMes );
    }
    else if ( pY->Y_Type == id_FF_AO )
    {
      SetModel( pY->Name, CY_DI::eptAnalog, 'O', Model, Point, cMes, pY->pDR->NumbFCS, pY->pDR->domain );
      fprintf ( F, "%s;;AO;%s;%s;;%s;;%s;\n", pY->Name, pY->pComm, Model, Point, cMes );
    }
    else if ( pY->Y_Type == id_FF_DI )
    {
      SetModel( pY->Name, CY_DI::eptDigital, 'I', Model, Point, cMes, pY->pDR->NumbFCS, pY->pDR->domain );
      fprintf ( F, "%s;;DI;%s;%s;;%s;;%s;\n", pY->Name, pY->pComm, Model, Point, cMes );
    }
    else if ( pY->Y_Type == id_FF_DO )
    {
      SetModel( pY->Name, CY_DI::eptDigital, 'O', Model, Point, cMes, pY->pDR->NumbFCS, pY->pDR->domain );
      fprintf ( F, "%s;;DO;%s;%s;;%s;;%s;\n", pY->Name, pY->pComm, Model, Point, cMes );
    }
    else if ( pY->Y_Type == id_FF_MAI )
    {
      CY_FF_MAI *pFF = (CY_FF_MAI*)pY;
      SetModel( pY->Name, CY_DI::eptAnalog, 'I', Model, Point, cMes, pY->pDR->NumbFCS, pY->pDR->domain );
      for( int i=0; i<pFF->kOut; i++ )
      {
        if( !pFF->Out[i].pTo[0] )
          continue;
        fprintf ( F, "%s.%s_V;;AI;%s(%s);;;%s;;;\n", pY->Name, pFF->Out[i].Name, pY->pComm, pFF->Out[i].pTo[0], Point );
      }
    }
    else if( id_BOPCBOL==pY->Y_Type )
    {
      CY_BOPCBOL *pOPC = (CY_BOPCBOL*)pY;
      SetModel( pOPC->Name, CY_DI::eptDigital, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;D_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCI16==pY->Y_Type )
    {
      CY_BOPCI16 *pOPC = (CY_BOPCI16*)pY;
      SetModel( pOPC->Name, CY_DI::eptDigital, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;D_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCU16==pY->Y_Type )
    {
      CY_BOPCU16 *pOPC = (CY_BOPCU16*)pY;
      SetModel( pOPC->Name, CY_DI::eptDigital, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;D_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCI32==pY->Y_Type )
    {
      CY_BOPCI32 *pOPC = (CY_BOPCI32*)pY;
      SetModel( pOPC->Name, CY_DI::eptDigital, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;D_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCU32==pY->Y_Type )
    {
      CY_BOPCU32 *pOPC = (CY_BOPCU32*)pY;
      SetModel( pOPC->Name, CY_DI::eptDigital, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;D_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCF32==pY->Y_Type )
    {
      CY_BOPCF32 *pOPC = (CY_BOPCF32*)pY;
      SetModel( pOPC->Name, CY_DI::eptAnalog, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;A_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
    else if( id_BOPCF64==pY->Y_Type )
    {
      CY_BOPCF64 *pOPC = (CY_BOPCF64*)pY;
      SetModel( pOPC->Name, CY_DI::eptAnalog, '_', Model, Point, cMes, pOPC->pDR->NumbFCS, pOPC->pDR->domain );
      fprintf ( F, "%s;;A_;%s;%s;;%s;;%s;\n", pOPC->Name, pOPC->pComm, Model, Point, cMes );
    }
  }
////////////////////////////////////////////////////////////////////////////////
  return 0;
}

CY_Base* CY_MainLogic::SetModel( char * Y, CY_DI::ePointType eptModelType, char IO, char (&Model)[256], char (&Point)[256], char (&cMes)[64], int NumbFCS, int domain )
{
  Model[0] = 0;
  Point[0] = 0;
  cMes[0] = 0;
  CY_Base *pObj = NULL;

  if( IO=='I' || IO=='O')
  {
    BYTE lenY = strlen( Y );
    for( int i=0; i<kConnection; i++ )
    {
      CObjConnection &con = Connection[i];
      LPCSTR pt = con.Out;
      int len = con.PntOut;
      if( IO=='O' )
      {
        pt = con.In;
        len = con.PntIn;
      }

      bool bSamename = false;
      if( len > 0 )
      {
        if( len == lenY )
          bSamename = !strncmp( pt, Y, len );
      }
      else
        bSamename = !strcmp( pt, Y );

      if( bSamename && -1!=NumbFCS && con.NumbFCS==NumbFCS && con.domain==domain )
      {
        char objCon[36];
        if( IO=='I' )
        {
          strncpy_s( objCon, con.In, con.PntIn );
          objCon[con.PntIn] = 0;
        }
        else
        {
          strncpy_s( objCon, con.Out, con.PntOut );
          objCon[con.PntOut] = 0;
        }

        pObj = FindYObj( objCon );
        if( !pObj )
          continue;
        //if( eptModelType==CY_DI::eptAnalog)
        {
          char *pVal = NULL;
          void * pTuneVar = NULL;
          CY_Base::eVarType Work = CY_Base::evtP;
          char TypeVar = ' ';
          bool r = pObj->GetVar( "cMes", (void**)&pVal, &pTuneVar, &TypeVar, &Work );
          if( r )
          {
            ASS( TypeVar=='S');
            strcpy_s( cMes, pVal );
          }
        }
        if( cMes[0] )
          break;
      }
    }
  }
#ifdef _WIN64
  if( !strcmp( cMes, "%") )
    cMes[0] = 0;
#endif
  return pObj;
}
