#include "Y_MainLogic.h"
#include "_calcu.h"

void __declspec(dllimport) GetST16_LC64CondFunc( char * Cond, CCond_ST16_LC64 * pVC, CY_Base *pObj );
void __declspec(dllimport) GetST16ActFunc( char * Act, CAct_ST16 * pVA, CY_Base *pObj );

int CY_MainLogic::Step0L()
{
  OutputDebugString( "Y Step0...\n" );
  DWORD s = GetTickCount();

  for ( int n = 0; n < kY_Obj; ++n )
  {
    pY_Obj[n]->LinkToDI();
  }

  ObrConnections(  );
  CRefAny::InitAll();
  for ( int n = 0; n < kD_Obj; ++n )
    pD_Obj[n]->Prepare0();
  for ( int n = 0; n < kY_Obj; ++n )
  {
    pY_Obj[n]->Prepare0();
    pY_Obj[n]->OnParamsInitialized();//MC-NE изменяет HH/LL
  }
  for ( int n = 0; n < kDR; ++n )
  {
    CY_DR * pDR = DR[n];
    pDR->Prepare0();
  }

  DWORD d = GetTickCount() - s;
  char szDebug[64];
  sprintf_s( szDebug, "Y Prepare0 (%d ms)\n", d );
  OutputDebugString( szDebug );
  return 0;
}

template<size_t _C>
bool FillInfoSignalTo( LPSTR (&pTo)[_C], LPSTR pIn )
{
  int iSignalTo = -1;
  for( int i=0; i<_countof(pTo); i++ )
  {
    if( !pTo[i] )
    {
      iSignalTo = i;
      break;
    }
  }
  bool r = true;
  if( -1==iSignalTo )
  {
    iSignalTo = _countof(pTo) - 1;
    r = false;
  }
  if( !pTo[iSignalTo] )
    pTo[iSignalTo] = pIn;
  return r;
}

#include <sstream>
static CY_Base KEY( true );
static CY_Base * pKEY = &KEY;
void CY_MainLogic::ObrConnections(  )
{
  char NameObj[128];

  for ( int n = 0; n < kConnection; n++ )
  {
    CObjConnection * pC = &Connection[n];

    pC->Sig.Name = NULL;

    pC->ConnectionType = CObjConnection::ectUndefined;
    int pnt_in = pC->PntIn;
    int pnt_out = pC->PntOut;
    char * pOutVar = NULL;
    bool bComplexOut = false;
    bool bComplexIn = false;
    if ( pnt_out > 0 )
    {
      ASS ( pC->Out[0] != '>' )
      memcpy ( NameObj, pC->Out, pnt_out );
      NameObj[pnt_out] = 0;
      pOutVar = pC->Out + pnt_out + 1;
      LPCSTR p1 = strchr(pOutVar, '.');
      if( p1 )
        bComplexOut = true;
      else
      {
        p1 = strchr(pOutVar, '=');
        if( p1 )
          bComplexOut = true;
      }
    }
    else
    {
      strcpy_s( NameObj, pC->Out );
      pOutVar = "";
    }

    bool bRefOtherFcs = false;
    if( pC->dirIO=='I' )
      bRefOtherFcs = pC->bRefOtherFcsOut;

    LPSTR pszFcsSep = strchr( NameObj, '@');
    if( pszFcsSep && pszFcsSep > NameObj && '.'!=pszFcsSep[-1] )
    {
      *pszFcsSep = 0;
      bRefOtherFcs = true;
    }

    pC->pOutObject = FindYObj( NameObj, !bRefOtherFcs, pC->NumbFCS, pC->domain);
    if( !pC->pOutObject )
      pC->pOutObject = GetObj( NameObj, pC->NumbFCS, pC->domain, !bRefOtherFcs );

    if( !pC->pOutObject && bRefOtherFcs )
    {
      std::stringstream ss;
      ss << n << " Не найти объект-источник "  << pC->Out << " на другом контроллере, пробую на этом же\r\n";
      OutputDebugStringA( ss.str().c_str() );
      pC->pOutObject = FindYObj( NameObj, true, pC->NumbFCS, pC->domain);
      if( !pC->pOutObject )
        pC->pOutObject = GetObj( NameObj, pC->NumbFCS, pC->domain, true );
      if( !pC->pOutObject )
      {
        std::stringstream ss2;
        ss2 << n << " Не найти объект-источник "  << pC->Out << " и на этом контроллере\r\n";
        OutputDebugStringA( ss2.str().c_str() );
      }
    }

    if( !pC->pOutObject && s_bFcsMultiDomain )
    {
      std::stringstream ss;
      ss << n << " Пробую искать "  << pC->Out << " в другом домене ";
      pC->pOutObject = (*pFindYObjN)( NameObj, pC->NumbFCS, -pC->domain);
      if( pC->pOutObject )
        ss << "- найден";
      ss << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
    }

    if( !pC->pOutObject )
    {
      std::stringstream ss;
      ss << n << " Не найти объект-источник "  << pC->Out << " для связи с " << pC->In << " на FCS " << pC->domain << " " << pC->NumbFCS << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
      //if( F_err )Fprintf( F_err, ss.str().c_str() );
      continue;
    }

    char * pInVar = NULL;
    if ( pnt_in > 0 )
    {
      memcpy ( NameObj, pC->In, pnt_in );
      NameObj[pnt_in] = 0;
      pInVar = pC->In + pnt_in + 1;
      LPCSTR p1 = strchr(pInVar, '.');
      if( p1 )
        bComplexIn = true;
    }
    else
    {
      strcpy_s( NameObj, pC->In );
      pInVar = "";
    }
    bRefOtherFcs = false;
    if( pC->dirIO=='O' )
      bRefOtherFcs = pC->bRefOtherFcsIn;
    pC->pInObject = FindYObj( NameObj, !bRefOtherFcs, pC->NumbFCS, pC->domain );
    if( !pC->pInObject )
      pC->pInObject = GetObj( NameObj, pC->NumbFCS, pC->domain, !bRefOtherFcs );

    if( !pC->pInObject && bRefOtherFcs )
    {
      std::stringstream ss;
      ss << n << " Не найти In объект "  << pC->In << " на другом контроллере, пробую на этом же\r\n";
      OutputDebugStringA( ss.str().c_str() );
      //if( F_err ) Fprintf( F_err, ss.str().c_str() );
      pC->pInObject = FindYObj( NameObj, true, pC->NumbFCS, pC->domain);
      if( !pC->pInObject )
        pC->pInObject = GetObj( NameObj, pC->NumbFCS, pC->domain, true );
    }

    //ASS( pC->pInObject );

    if ( pC->pInObject == NULL || pC->pOutObject == NULL )
    {
      std::stringstream ss;
      ss << n << " Не найти объекты для связи " << pC->Out;
      if( !pC->pOutObject )
        ss << "(NULL)";
      ss << " -> " << pC->In;
      if( !pC->pInObject )
        ss << "(NULL)";
      ss << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
      //if( F_err ) Fprintf( F_err, ss.str().c_str() );
      continue;
    }

    ////////////////////////////////////////////////////////////////////////
    // Сигнал
    if( strcmp ( pC->pOutObject->Model, "SW-33" ) == 0 )
    {
      CY_SW_33 * p33 = (CY_SW_33*)pC->pOutObject;
      CSignal ** pSigIn = pC->pInObject->GetIN( pInVar );
      if( pSigIn )
      {
        bool r = p33->SetOUT ( pOutVar, pSigIn );
        if( r )
        {
          pC->ConnectionType = CObjConnection::ectSignal2Signal;
          pC->TypeOut = 's';
          pC->TypeIn = 's';
          continue;
        }
      }
      else
      {
        void * pVar = NULL;
        void * pTuneVar = NULL;
        CY_Base::eVarType Work = CY_Base::evtP;
        bool r = pC->pInObject->GetVar( pInVar, &pVar, &pTuneVar, &pC->TypeIn, &Work, NULL, &pC->sizeIn );
        if( !r )
        {
          std::stringstream ss;
          ss << n << " Не связать " << pC->Out;
          ss << " -> " << pC->In;
          ss << ", т.к. не найти входную точку " << pInVar;
          ss << "\r\n";
          OutputDebugStringA( ss.str().c_str() );
          //if( F_err ) Fprintf( F_err, ss.str().c_str() );
          continue;
        }
        else if( CY_Base::evtSignal==Work )
        {
          std::stringstream ss;
          ss << n << " Не связать " << pC->Out;
          ss << " -> " << pC->In;
          ss << ", т.к. выдан сигнал " << pInVar;
          ss << "\r\n";
          OutputDebugStringA( ss.str().c_str() );
          //if( F_err ) Fprintf( F_err, ss.str().c_str() );
          continue;
        }
        pC->Sig.Name = pInVar;
        if( !ConnectVar2Signal( pVar, pC->TypeIn, pC->Sig, pC->sizeIn ) )
        {
          r = false;
          pC->Sig.Name = NULL;
          continue;
        }
        pC->Sig.pRef = &pC->Sig;
        pC->ConnectionType = CObjConnection::ectSignal2Signal;
        pC->TypeOut = 's';
        pC->TypeIn = 's';
        p33->SetOUT( pOutVar, &pC->Sig.pRef );
        continue;
      }
    }
    if( strcmp ( pC->pInObject->Model, "SW-33" ) == 0 )
    {
      CY_SW_33 * p33 = (CY_SW_33*)pC->pInObject;
      CSignal * pSigOut = pC->pOutObject->GetOUT( pOutVar );
      bool r = false;
      if( !pSigOut )
      {
        void * pVar = NULL;
        void * pTuneVar = NULL;
        CY_Base::eVarType Work = CY_Base::evtP;
        USHORT varOutSize = 0;
        r = pC->pOutObject->GetVar( pOutVar, &pVar, &pTuneVar, &pC->TypeOut, &Work, NULL, &varOutSize );
        if( r && Work!=CY_Base::eVarType::evtSignal )
        {
          pC->Sig.Name = pOutVar;
          if( ConnectVar2Signal( pVar, pC->TypeOut, pC->Sig, varOutSize ) )
          {
            pSigOut = &pC->Sig;
            r = p33->SetIN ( pInVar, pSigOut );
          }
          else
          {
            r = false;
            pC->Sig.Name = NULL;
          }
        }
      }
      if( pSigOut )
        r = p33->SetIN ( pInVar, pSigOut );
      else
      {
        std::stringstream ss;
        ss << n << " На вход SW-33 " << pC->In << " не получить выход с " << pC->Out << "\n";
        OutputDebugStringA( ss.str().c_str() );
      }
      if( r )
      {
        pSigOut->pFrom = pC->Out;
        if( !FillInfoSignalTo(pSigOut->pTo, pC->In ) )
        {
          std::stringstream ss;
          ss << n << " Не достаточно места для информации о связи " << pC->Out << " -> " << pC->In << ", т.к. нет свободного выхода\n";
          OutputDebugStringA( ss.str().c_str() );
        }
        pSigOut->On( CSignal::USED );
        pC->ConnectionType = CObjConnection::ectSignal2Signal;
        pC->TypeOut = 's';
        pC->TypeIn = 's';
      }
      else
      {
        KKK();
      }
      if( pSigOut && pC->pOutObject )
        pSigOut->Y_Type = pC->pOutObject->Y_Type;
      continue;
    }
    CSignal * pSigOut = NULL;
    if( bComplexOut )
    {
      pC->condition.Name = pC->Out;
      GetST16_LC64CondFunc( pOutVar, &pC->condition, pC->pOutObject  );
      if( !pC->condition.VarType || pC->condition.VarType==' ' )
      {
        ASS(0);
        continue;
      }
      pC->ConnectionType = CObjConnection::ectCondition2Signal;
      pC->TypeOut = 'c';
      pC->Sig << pC->condition.Yes;
      pSigOut = &pC->Sig;
    }
    else
    {
      if( !pC->bRefOtherFcsOut )//взять сигнал можно только с этого контроллера
        pSigOut = pC->pOutObject->GetOUT( pOutVar );
    }
    if( pSigOut && pC->pOutObject )
      pSigOut->Y_Type = pC->pOutObject->Y_Type;

    CSignal ** pSigIn = NULL;
    if( bComplexIn )
    {
      pC->action.Name = pC->In;
      GetST16ActFunc( pInVar, &pC->action, pC->pInObject );
      if( !pC->action.VarType || pC->action.VarType==' ' )
      {
        ASS(0);
        continue;
      }
      pC->ConnectionType = CObjConnection::ectSignal2Action;
      pC->pIn = &pC->bAction;
      pC->TypeIn = 'B';
    }
    else
      pSigIn = pC->pInObject->GetIN( pInVar );
    ////////////////////////////////////////////////////////////////////////
    if( pSigOut && id__Z==pSigOut->Y_Type )
      pSigOut->On( CSignal::Z_Connection );

    if( pSigOut && pSigIn )
    {
      // Оба сигнала
      pSigOut->pFrom = pC->Out;
      if( !FillInfoSignalTo(pSigOut->pTo, pC->In ) )
      {
        std::stringstream ss;
        ss << n << " Не достаточно места для информации о связи " << pC->Out << " -> " << pC->In << ", т.к. нет свободного выхода\n";
        OutputDebugStringA( ss.str().c_str() );
      }
      if( pC->ConnectionType==CObjConnection::ectUndefined )
        pC->ConnectionType = CObjConnection::ectSignal2Signal;
      if( pC->TypeOut==0 )
        pC->TypeOut = 's';
      pC->TypeIn = 's';
      *pSigIn = pSigOut;
      pSigOut->On( CSignal::USED );
      ChackConnection( pC );
      continue;
    }
    ////////////////////////////////////////////////////////////////////////
    if( pSigIn )
    {
      // На вход - сигнал
      *pSigIn = &pC->Sig;
      pC->Sig.On( CSignal::USED );
      pC->Sig.pFrom = pC->Out;
      ASS( !pC->Sig.pTo[0] );
      pC->Sig.pTo[0] = pC->In; 
      pC->TypeIn = 's';
      void * pVar = NULL;
      void * pTuneVar = NULL;
      CY_Base::eVarType Work = CY_Base::evtP;
      USHORT varOutSize = 0;
      bool Res = pC->pOutObject->GetVar ( pOutVar, &pVar, &pTuneVar, &pC->TypeOut, &Work, NULL, &varOutSize );
      if ( !Res )
      {
        *pSigIn = NULL;
        std::stringstream ss;
        ss << n << " Не найти выходную точку для связи " << pC->Out << " -> " << pC->In << "\r\n";
        OutputDebugStringA( ss.str().c_str() );
        //if( F_err ) Fprintf( F_err, ss.str().c_str() );
        continue;
      }
      if( ConnectVar2Signal( pVar, pC->TypeOut, pC->Sig, varOutSize ) )
      {
        pC->ConnectionType = CObjConnection::ectVar2Signal;
        if( id__Z!=pC->pOutObject->Y_Type )
          (*pSigIn)->On( CSignal::NONE_LIMITED );
      }
      ChackConnection( pC );
      if( pSigOut )
        pSigOut->numConnection = n;      
      continue;
    }
    ////////////////////////////////////////////////////////////////////////
    if( pSigOut )
    {
      pSigOut->pFrom = pC->Out;
      if( !FillInfoSignalTo(pSigOut->pTo, pC->In ) )
      {
        std::stringstream ss;
        ss << n << " Не достаточно места для информации о связи " << pC->Out << " -> " << pC->In << ", т.к. нет свободного выхода\n";
        OutputDebugStringA( ss.str().c_str() );
      }
      pSigOut->On( CSignal::USED );
      pC->Sig.pRef = pSigOut;
      // C выхода - сигнал
      ConnectSignal2Var( &pC->pOut, pC->TypeOut, *pSigOut );
      if( pC->ConnectionType==CObjConnection::ectUndefined )
      {
        void * pTuneVar = NULL;
        CY_Base::eVarType Work = CY_Base::evtP;
        bool Res = pC->pInObject->GetVar( pInVar, &pC->pIn, &pTuneVar, &pC->TypeIn, &Work, NULL, &pC->sizeIn );
        if ( !Res )
        {
          std::stringstream ss;
          ss << n << " Не найти входную точку для связи " << pC->Out << " -> " << pC->In << "\r\n";
          OutputDebugStringA( ss.str().c_str() );
          //if( F_err ) Fprintf( F_err, ss.str().c_str() );
          continue;
        }
        if ( pC->TypeIn == pC->TypeOut )
          pC->ConnectionType = CObjConnection::ectSignalMove2Var;
        else
          pC->ConnectionType = CObjConnection::ectSignalConvert2Var;
      }
      else if( pC->ConnectionType==CObjConnection::ectSignal2Action )
      {
        if ( pC->TypeIn == pC->TypeOut )
          pC->ConnectionType = CObjConnection::ectSignalMove2Action;
        else
          pC->ConnectionType = CObjConnection::ectSignalConvert2Action;
      }
      else
      {
        ASS(!"Такого быть не должно");
      }
      ChackConnection( pC );
      if( pSigOut )
        pSigOut->numConnection = n;
      continue;
    }
    ////////////////////////////////////////////////////////////////////////
    void * pTuneVar = NULL;
    CY_Base::eVarType Work = CY_Base::evtP;
    bool Res = pC->pOutObject->GetVar( pOutVar, &pC->pOut, &pTuneVar, &pC->TypeOut, &Work );
    if ( !Res )
    {
      std::stringstream ss;
      ss << n << " Не найти выходную точку для связи " << pC->Out << " -> " << pC->In << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
      continue;
    }
    Res = pC->pInObject->GetVar( pInVar, &pC->pIn, &pTuneVar, &pC->TypeIn, &Work, NULL, &pC->sizeIn );
    if ( !Res )
    {
      std::stringstream ss;
      ss << n << " Не найти входную точку для связи " << pC->Out << " -> " << pC->In << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
      //if( F_err ) Fprintf( F_err, ss.str().c_str() );
      continue;
    }
    ASSD(Work)
    if ( pC->TypeIn == pC->TypeOut )
      pC->ConnectionType = CObjConnection::ectSignalMove2Var;
    else
      pC->ConnectionType = CObjConnection::ectSignalConvert2Var;
    ChackConnection( pC );
    if( pSigOut )
      pSigOut->numConnection = n;
  }
  //if( F_err ) Fclose ( F_err );
}

void CY_MainLogic::ChackConnection( CObjConnection * pC )
{
  int pnt_in = pC->PntIn;
  int pnt_out = pC->PntOut;
  char * pInVar = NULL;
  char * pOutVar = NULL;

  if( pnt_in > 0 )
    pInVar = pC->In + pnt_in + 1;
  if ( pnt_out > 0 )
    pOutVar = pC->Out + pnt_out + 1;

  if( !pInVar || !pOutVar )
    return;

  if( !strcmp( pC->pInObject->Model, "INDST3" )
    && !strcmp( pC->pOutObject->Model, "DSET" ) )
  {
    if( !strcmp( pInVar, "JSV") && !strcmp( pOutVar, "SV") )
    {
      pC->Sig.On( CSignal::_ReverseExchange );
      std::stringstream ss;
      ss << " Изменение обмена данными для "  << pC->In << " <->> " << pC->Out << "\r\n";
      OutputDebugStringA( ss.str().c_str() );
    }
  }
}

bool CY_MainLogic::ConnectSignal2Var( void **pVar, char &TypeVar, CSignal &Sig )
{
  switch( Sig.Type )
  {
  case CSignal::Int:
    TypeVar = 'I';
    *pVar = Sig.pInt;
    break;
  case CSignal::Double:
    TypeVar = 'D';
    *pVar = Sig.pDbl;
    break;
  case CSignal::Bool:
    TypeVar = 'B';
    *pVar = Sig.pBool;
    break;
  case CSignal::NoType:
    TypeVar = 'I';
    *pVar = Sig.pInt;
    break;
  case CSignal::Byte:
    TypeVar = 'W';
    *pVar = Sig.pByte;
    break;
  default:
    ASS(FALSE)
      return false;
  }
  return true;
}

bool CY_MainLogic::ConnectVar2Signal( void *pVar, char TypeVar, CSignal &Sig, USHORT varSize )
{
  switch( TypeVar )
  {
  case 'I':
    Sig << *(int*)pVar;
    break;
  case 'D':
    Sig << *(double*)pVar;
    break;
  case 'B':
    Sig << *(bool*)pVar;
    break;
  case 'W':
    Sig << *(BYTE*)pVar;
    break;
  case 'S':
    Sig.pChar = (char*)pVar;
    Sig.Type = CSignal::String;
    Sig.stringSize = varSize;
    break;
  default:
    ASS(FALSE)
      return false;
  }
  return true;
}
