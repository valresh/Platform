#include "stdafx.h"
#include "Y_Data.h"
#include <SetDataTypes.h>
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>
#include "FlagsSupport.h"

static CY_Create PIO ( "PIO", CY_IO::Create );

CY_IO_W::CY_IO_W()
{
  ZeroMemory( this, sizeof(*this) );
}

void CY_IO::InitData()
	{
	pDR = NULL;
  pClassBase = NULL;
  ClassParmInfo = NULL;
  kClassParmInfo = 0;
  ClassVarInfo = NULL;
  kClassVarInfo = 0;
  pClass_P = NULL;
  L_Class_P = 0;
  Mes = -1;
	}

CY_IO::CY_IO() : CY_Base()
{
	InitData();
	Type = id_Unknown;
	strcpy_s( Model, "PIO" );
	IN = NULL;
  ZeroMemory( ins, sizeof(ins) );
  OUT.Name = "OUT";
	pDI = NULL;
	WorkModel = false;
	TypePV = None;
	pFound = &Is;
  Start = false;
  Mes = -1;
	Get_Mes = false;
	//iPV_old = -1;
	NoPrepare = false;
	ConstVal_D = 0.;
	ConstVal_I = 0;
	Gain = 1.;
	Bias = 0.;
	MashtabL = 100.;
	LocalActions = false;
	SetDefect = false;
	PV_Z = -1.;
  ppRefSignal = NULL;
  pVarRef = NULL;
	pAN = NULL;
  FlagUse = 0;
  Is = false;
  CY_Base::InitFlagUse( FlagUse );
}

int GetNewName ( char Old[16], char New[16] );

int CY_IO::SetData( int TypeData, void * pData )
  {
  return 0;
  }

CSignal * CY_IO::GetOUT ( char * OutName )
	{
  return &OUT;
	}

bool CY_IO::SetLimits( )
  {
  /*if ( IN )
    {
    IN ->Min = 0.;
    IN ->Max = 100.;
    IN ->IsLimits = true;
    }*/
  return true;
  }

CSignal ** CY_IO::GetIN ( char * NameIN )
{
  if( IN )
	{
    for ( int i=0; i<_countof(ins); i++ )
    {
      if( !ins[i] )
        return &ins[i];
	}
    ASS(0);
  }
 	return &IN;
}

void CY_IO::LinkToDI()
{
  Prepare1();
  if( !pVarRef && !ppRefSignal )
    {
    OutputDebugString( "нет ссылки на DIO у точки ");
    OutputDebugString( Name );
    if( pDR )
    {
      OutputDebugString( "   " );
      OutputDebugString( pDR->FCS );
    }
    OutputDebugString( "\n" );
    }
}

bool CY_IO::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse/*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  if( pDI )
    return pDI->GetVar ( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  return false;
}

template<size_t _S>
int GetMaxVal( CSignal *in, CSignal *(&ins)[_S] )
{
  int v = 0;
  if( in )
    *in >> v;
  for ( size_t i=0; i<_S; i++)
  {
    if( !ins[i] )
      break;
    int t = 0;
    *ins[i] >> t;
    if( t>v )
      v = t;
  }
  return v;
}

void CY_IO::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
  if( ppRefSignal )
  {
    if( IN )
      KKK();
  }
  if( !pVarRef )
    return;
  if( IN )
  {
    switch( TypePV )
    {
    case Int:
      {
        const int iPV = GetMaxVal( IN, ins );
        int &pvInZ = *(int*)pVarRef;

        if( id_PBS5C==IN->Y_Type || id_PBS10C==IN->Y_Type )
        {
          if( pDI && (id__Z==pDI->Y_Type || id__Y==pDI->Y_Type) )
          {
            if( iPV!=_iPV_old )
              pvInZ = iPV;
            else if( iPV != pvInZ )
              *IN = pvInZ;
          }
          else
            pvInZ = iPV;
        }
        else
          pvInZ = iPV;

        _iPV_old = iPV;
      }
      break;
    case Double:
      *IN >> *(double*)pVarRef;
      break;
    default:
      ASSD(0);
    }
  }

  if( TypePV == Double )
  {
    *(double*)pVarRef = *(double*)pVarRef / 100. * MashtabL;
  }

  if( (FlagUse&CAlarmBase::fuYConnect) || (pDI && (pDI->FlagUse&CAlarmBase::fuYConnect)) )
  {
    FlagUse |= CAlarmBase::fuYConnect;
    if( pDI )
      pDI->FlagUse |= CAlarmBase::fuYConnect;
  }
  //	
  switch( Type )
  {
  case id_ObjRef:
    if( IntCompareWith==TypePV )
      compareResultOfI = (*(int*)pVarRef)==ConstVal_I ? 1 : 0; 
    else if( FlagByteCompareWith==TypePV )
      compareResultOfI = (*(BYTE*)pVarRef)==ConstVal_I ? 1 : 0;
    if( DblCompareWith==TypePV )
      compareResultOfI = abs( (*(double*)pVarRef)-ConstVal_D) < 0.0000001 ? 1 : 0; 
    break;
  case id__Z:
    {
      if (  pDI->pointTypeZ == CY_DI::eptAnalog )
      {
        if ( pDI->IOZ == 'I' )
        {
            //ASS( !pDI->bConnectedByACS );
          {
          }
        }
        else if ( IN )
        {
          if ( PV_Z != -1. )
            *(double*)pVarRef = PV_Z;
          else
            *(double*)pVarRef = *(double*)pVarRef * Gain + Bias;
          IN->BackVar = *(double*)pVarRef;
        }
        break;
      }
      if (  pDI->pointTypeZ == CY_DI::eptDigital )
      {
        if ( pDI->IOZ == 'I' )
        {
          if ( PV_Z != -1. )
            *(int*)pVarRef = (int)PV_Z;
        }
        else
        {
          if ( IN )
          {
            if ( PV_Z != -1. )
              *(int*)pVarRef = (int)PV_Z;
          }
          else
          {
            if ( PV_Z != -1. )
              *(int*)pVarRef = (int)PV_Z;
          }
        }
        break;
      }
      ASS(0)
    }
    break;
  }
}

void CY_IO::Init( )
{
}

static BYTE s_Const_No = 0;

bool CY_IO::GetObj( bool Out )
{
  if ( Type != id_Unknown )
    return true;
  if ( Name[0] == '%' && Name[1] == '%' )
  {
    ASS( pDR );
    pDI = (*pFindDObj)( Name, pDR->NumbFCS, pDR->domain );
#pragma message("много не существующих точек")
    if( !pDI )
    {
      LPSTR pSep = strchr(Name, '.');
      if( pSep )
      {
        TStringTerminator st(pSep);
        ++pSep;
        pDI = (*pFindDObj)( Name, pDR->NumbFCS, pDR->domain );
      }
    }
//    ASS( pDI );
    if( pDI == NULL )
    {
      // Нестандартный IO
      LPSTR pSep = strchr(Name, '.');
      if( !pSep )
        return false;
      TStringTerminator st(pSep);
      ++pSep;
      CY_Base *pObj = pFindYObj( Name, pDR->domain );
      if( !pObj || pObj==this )
        return false;
      void * pTuneVar = NULL;
      char varType = ' ';
      CY_Base::eVarType Work = CY_Base::evtP;
      bool Res = pObj->GetVar( pSep, &pVarRef, &pTuneVar, &varType, &Work );
      ASS( Res );
      if( !Res )
        return false;
      switch( varType )
      {
      case 'I':
        TypePV = Int;
        OUT << *(int*)pVarRef;
        break;
      case 'D':
        TypePV = Double;
        OUT << *(double*)pVarRef;
        break;
      default:
        ASS( 0 );
        pVarRef = NULL;
        return false;
      }
      if( id__Z==pObj->Y_Type)
      {
        OUT.On( CSignal::Z_Connection );
        ASSD( 'Z'==pObj->Model[0] && 0==pObj->Model[1] );
      }
      if( id__Y==pObj->Y_Type)
      {
        OUT.On( CSignal::Z_Connection );
        ASSD( 'Y'==pObj->Model[0] && 0==pObj->Model[1] );
      }
      return true;
    }
    //
    Type = (EDataTypes)pDI->Y_Type;
    if ( pDI->pointTypeZ == CY_DI::eptAnalog )
    {
      ASS( pDI->pDPV );
      TypePV = Double;
      pVarRef = pDI->pDPV;
      OUT << *pDI->pDPV;
    }
    else if( /*pDI->TypeZ == 'I' ||*/ pDI->pointTypeZ == CY_DI::eptDigital )
    {
      ASS( pDI->pPV );
      TypePV = Int;
      pVarRef = pDI->pPV;
        OUT << *pDI->pPV;
    }
    else
    {
      ASS(0);
    }

    return true;
  }
  if ( strncmp ( Name, "%GS", 3 ) == 0 )
    {
    Type = id__GS;
    int Numb;
    int NumbFCS;
    if ( Name[6] )
      {
      Numb = atoi ( Name + 3 );
      NumbFCS = Numb % 100;
      Numb = Numb / 100;
      }
    else
      {
      Numb = atoi ( Name + 3 );
      NumbFCS = pDR->NumbFCS;
      }
    pDI = (CY_DI*)(*pFindDI)( NumbFCS, pDR->domain, Numb, Type );
    if( !pDI )
      return false;
    ASS( Type == (EDataTypes)pDI->Y_Type );
    CY_Base::InitFlagUse( pDI->FlagUse );
    TypePV = Int;
    pVarRef = &( pDI->pGS->PV );
    return true;
    }
  if ( strncmp ( Name, "%SW", 3 ) == 0 )
  {
    Type = id__SW;
    int NumbFCS;
    int NumbSW = 0;
    if ( Name[7] && '.'!=Name[7] )
      {
      NumbSW = atoi ( Name + 3 );
      NumbFCS = NumbSW % 100;
      NumbSW = NumbSW / 100;
      }
    else
      {
      NumbSW = atoi ( Name + 3 );
      NumbFCS = pDR->NumbFCS;
      }
    pDI = (CY_DI*)(*pFindDI)( NumbFCS, pDR->domain, NumbSW, Type );
    ASS( pDI );
    if( !pDI )
      return false;
    ASS( Type == (EDataTypes)pDI->Y_Type );
    CY_Base::InitFlagUse( pDI->FlagUse );
    TypePV = Int;
    pVarRef = &( pDI->pSW->PV );
    if( pDI->pointTypeZ==CY_DI::eptUnknown )
      pDI->pointTypeZ = CY_DI::eptDigital;
    return true;
  }
  if ( strncmp ( Name, "%WB", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Type = id__WB;
    if( Name[9] && '.'!=Name[9] )
    {
      ASSD(0);
      Numb = atoi ( Name + 3 );
      NumbFCS = Numb % 100;
      Numb = Numb / 100;
    }
    else
    {
      Numb = atoi ( Name + 3 );
      NumbFCS = pDR->NumbFCS;
    }
    pDI = (*pFindDI)( NumbFCS, pDR->domain, Numb, id__WB );
    //ASS( pDI );
    if( !pDI )
      return false;
    ASS( Type == (EDataTypes)pDI->Y_Type );
    CY_Base::InitFlagUse( pDI->FlagUse );
    TypePV = Int;
    pVarRef = &( pDI->pWB->PV );
    if( pDI->pointTypeZ==CY_DI::eptUnknown )
      pDI->pointTypeZ = CY_DI::eptDigital;
    return true;
  }
  if ( strncmp ( Name, "%WW", 3 ) == 0 )
  {
    Type = id__WW;
    int Numb;
    int NumbFCS;
    Numb = atoi( Name + 3 );
    NumbFCS = pDR->NumbFCS;
    pDI = (*pFindDI)( NumbFCS, pDR->domain, Numb, Type );
    ASS( pDI );
    if( !pDI )
      return false;
    ASS( Type == (EDataTypes)pDI->Y_Type );
    CY_Base::InitFlagUse( pDI->FlagUse );
    if ( pDI->pointTypeZ == CY_DI::eptAnalog )
    {
      TypePV = Double;
      pVarRef = &(pDI->pZ->DPV );
    }
    else
    {
      TypePV = Int;
      pVarRef = &( pDI->pZ->PV );
    }
    return true;
  }
  if ( strncmp ( Name, "%XW", 3 ) == 0 )
  {
    Type = id__XW;
    int Numb;
    int NumbFCS;
    Numb = atoi( Name + 3 );
    NumbFCS = pDR->NumbFCS;
    pDI = (*pFindDI)( NumbFCS, pDR->domain, Numb, Type );
    ASS( pDI );
    if( !pDI )
      return false;
    ASS( Type == (EDataTypes)pDI->Y_Type );
    CY_Base::InitFlagUse( pDI->FlagUse );
    if ( pDI->pointTypeZ == CY_DI::eptAnalog )
    {
      TypePV = Double;
      pVarRef = &(pDI->pZ->DPV );
    }
    else
    {
      TypePV = Int;
      pVarRef = &( pDI->pZ->PV );
    }
    return true;
  }
  if ( strncmp ( Name, "%Z", 2 ) == 0 )
  {
    int NumbSW = atoi ( Name + 2 );
    int NumbFCS = pDR->NumbFCS;
    //================================================
    pDI = (CY_DI*)(*pFindDI)( NumbFCS, pDR->domain, NumbSW, id__Z );
    //ASSD( pDI );
    if ( pDI == NULL )
      return false;//??
    CY_Base::InitFlagUse( pDI->FlagUse );
    if( !pDI->pDR )
      pDI->pDR = pDR;
    ASS( pDI->Y_Type == id__Z );
    C_Z * pZ = pDI->pZ;
    Type = id__Z;
    if ( pDI->pointTypeZ == CY_DI::eptAnalog )
      {
      TypePV = Double;
      pVarRef = &(pDI->pZ->DPV );
      }
    else
      {
      TypePV = Int;
      pVarRef = &( pDI->pZ->PV );
      }
    //================================================
    return true;
  }
  if ( strncmp ( Name, "%Y", 2 ) == 0 )
  {
    int NumbSW = atoi ( Name + 2 );
    int NumbFCS = pDR->NumbFCS;
    //================================================
    pDI = (CY_DI*)(*pFindDI)( NumbFCS, pDR->domain, NumbSW, id__Y );
    ASS( pDI );
    if ( pDI == NULL )
      return false;//??
    CY_Base::InitFlagUse( pDI->FlagUse );
    if( !pDI->pDR )
      pDI->pDR = pDR;
    ASS( pDI->Y_Type == id__Y );
    C_Y * pY = pDI->pY;
    Type = id__Y;
    if ( pDI->pointTypeZ == CY_DI::eptAnalog )
    {
      TypePV = Double;
      pVarRef = &(pDI->pY->DPV );
    }
    else
    {
      TypePV = Int;
      pVarRef = &( pDI->pY->PV );
    }
    //================================================
    return true;
  }
  ASS( pDR );

  char RefName[64];
  bool otherFCS = false;
  if ( Name[0] == '>' )
  {
    strcpy_s( RefName, Name + 1 );
    otherFCS = true;
  }
  else
    strcpy_s( RefName, Name );

  char * P = strchr ( RefName, '.' );

  LPSTR pszFcsSep = strchr( RefName, '@');
  if( pszFcsSep && pszFcsSep > RefName && '.'!=pszFcsSep[-1] )
    *pszFcsSep = 0;

  if( P )
  {
    TStringTerminator pst( P );
    CY_Base * pObj = (*pFindYObjN)( RefName, otherFCS ? -pDR->NumbFCS : pDR->NumbFCS, pDR->domain );
    CY_Base * pYobj = pObj;
    pDI = (CY_DI*)(*pFindDObj)( RefName, otherFCS ? -pDR->NumbFCS : pDR->NumbFCS, pDR->domain );
    if( pDI )
      pObj = pDI;
    if( pObj )
    {
      LPSTR pField = P + 1;
      LPSTR pNumb = strchr( pField, '.' );
      TStringTerminator st( pNumb );
      CYVarInfo * pI = GetParmRef ( pObj, pField );
      pVarRef = NULL;
      void * pTuneVar = NULL;
      CY_Base::eVarType Work = CY_Base::evtP;
      char TypeRef = ' ';
      if( !pObj->GetVar( pField, &pVarRef, &pTuneVar, &TypeRef, &Work ) )
      {
        if( pYobj && pObj!=pYobj )
        {
          if( !pObj->GetVar( pField, &pVarRef, &pTuneVar, &TypeRef, &Work ) )
          {
            ASSD(0);
            return false;
          }
        }
        else
        {
          ppRefSignal = pObj->GetIN(pField);
          if( ppRefSignal )
            return true;
          if( strstr(pField,"=BAD") )
          {
            pVarRef = &s_Const_No;
            TypePV = FlagByte;
            return true;
          }
          ASSD(0);
          return false;
        }
      }
      ASS( pVarRef )
      Type = id_ObjRef;
      TypePV = None;
      switch( TypeRef)
      {
      case 'D':
        TypePV = Double;
        if( pNumb )
        {
          TypePV = DblCompareWith;
          ConstVal_D = atof( pNumb+1 );
        }
        break;
      case 'I':
        TypePV = Int;
        if( pNumb )
        {
          TypePV = IntCompareWith;
          ConstVal_I = atoi( pNumb+1 );
        }
        break;
      case 'W':
        {
          TypePV = FlagByte;
          if( pNumb )
          {
            TypePV = FlagByteCompareWith;
            int n = GetAlrmNumber( pNumb+1 );
            if( n >= 0 )
              ConstVal_I = n;
            else
            {
              ASSD( 0 );
              pVarRef = NULL;
            }
          }
          else
          {
            ASSD(0);
          }
        }
        break;
      default:
        ASS(0);
        pVarRef = NULL;
        return false;
      }
      return true;
    }
    else
      KKK();
  }

  if( !pDI )
    pDI = (CY_DI*)(*pFindDObj)( RefName, otherFCS ? -pDR->NumbFCS : pDR->NumbFCS, pDR->domain );
  if( pDI )
  {
    CY_Base::InitFlagUse( pDI->FlagUse );
    Type = (EDataTypes)pDI->Y_Type;
    switch ( Type )
      {
    case id__Y:
      {
        TypePV = Int;
        pVarRef = &( pDI->pY->PV );
        if ( pDI->pointTypeZ == CY_DI::eptAnalog )
        {
          TypePV = Double;
          pVarRef = &( pDI->pY->DPV );
        }
      }
      break;
      case id__Z:
        {
        TypePV = Int;
        pVarRef = &( pDI->pZ->PV );
        if ( pDI->pointTypeZ == CY_DI::eptAnalog )
          {
          TypePV = Double;
          pVarRef = &( pDI->pZ->DPV );
          }
        }
        break;
      case id__SW:
        {
        TypePV = Int;
        pVarRef = &( pDI->pSW->PV );
        }
        break;
      case id__WB:
        {
        TypePV = Int;
        pVarRef = &( pDI->pWB->PV );
        }
        break;
      case id__AN:
        {
        TypePV = Int;
        pVarRef = &( pDI->pAN->PV );
        }
        break;
      case id__GS:
        {
        TypePV = Int;
        pVarRef = &( pDI->pGS->PV );
        }
        break;
      case id__PR:
        {
        TypePV = Int;
        pVarRef = &( pDI->pPR->PV );
        }
        break;
      default:
        ASS(FALSE)
        break;
      }
    return true;
  }
  // ;
  return false;
}

//#define SENS_TO_FILE
bool TestName ( char * Name );

int CY_IO::Prepare0()
	{
  GetMes();
	return 0;
	}

int CY_IO::Prepare1()
{
  if ( Type == id_Unknown )
    GetObj( true );
  if( pDI && id__Z==pDI->Y_Type)
  {
    OUT.On( CSignal::Z_Connection );
    ASSD( 'Z'==pDI->Model[0] && 0==pDI->Model[1] );
  }
  if( pDI && id__Y==pDI->Y_Type)
  {
    OUT.On( CSignal::Z_Connection );
    ASSD( 'Y'==pDI->Model[0] && 0==pDI->Model[1] );
  }
  if( pVarRef )
  {
    switch( TypePV )
    {
    case Int:
      _iPV_old = *(int*)pVarRef;
      OUT << *(int*)pVarRef;
      break;
    case Double:
      OUT << *(double*)pVarRef;
      break;
    case FlagByte:
      OUT << *(BYTE*)pVarRef;
      break;
    case IntCompareWith:
    case DblCompareWith:
    case FlagByteCompareWith:
      OUT << compareResultOfI;
      break;
    default:
      ASS( 0 );
    }
  }

  Start = true;
	WorkModel = true;
	return 0;
}

bool CY_IO::GetObj( int TYPE, void ** pData, char * Comm )
  {
  return false;
  }

int CY_IO::SaveState( )
{
	CY_Base::SaveState( );
	OUT.SaveState();
	SAVE_CLASS("W",CY_IO_W)
	return 0;
}

int CY_IO::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState( StrName ) == 0 )
	  return 0;
	REST_CLASS("W",CY_IO_W)
	return 1;
}

int CY_IO::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "compareResultOfI", compareResultOfI );
  return 0;
}

int CY_IO::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "compareResultOfI", compareResultOfI );
  return 0;
}

int CY_IO::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( WorkModel, "Работает" );
  if( pVarRef )
  {
    if( TypePV == Int )
    {
      PARM( *(int*)pVarRef, "#iPV" );
    }
    else if( TypePV == Double )
    {
      PARM ( *(double*)pVarRef, "#dPV" );
    }
  }
//	PARM ( Is, "#Найден" );
	PARM ( Mes, "Размерность(i)" );
//	PARM ( Get_Mes, "#Взять размерность" );
//  PARM ( dPV, "PV" );
//	PARM ( Gain, "Gain" );
//	PARM ( Bias, "Bias" );
//	PARM ( LocalActions, "Локальные действия" );
	PARM ( MashtabL, "Масштаб L мм" );
	if ( pDR && pDR->FCS )
    {
		PARM_CALLBACK( pDR->FCS, "#FCS" );
    } 
	MENU("Входы")
	PARM_IN(IN)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	//if ( Type == id_Sensor )
	//	{
	//	PARM ( PV_Z, "#Заданное PV" );
	//	}
	return 0;
	}

void CY_IO::GetMes()
  {
  }

static HBRUSH	hFonKlap = CreateSolidBrush( RGB ( 255, 192, 192 ));
static HBRUSH	hFonSens = CreateSolidBrush( RGB ( 192, 255, 192 ));

bool CY_IO::Draw ( CDrawData * pDD, int x, int y )
	{
	xInfo = 0;
	yInfo = 0;
	if (( Type != id_Klapan && Type != id_Sensor ) || !WorkModel )
		return false;
	RECT rc;
	rc.left = x;
	rc.top = y;
	rc.right = x + 144;
	rc.bottom = y + 24;
	/*if ( Type == id_Klapan )
	  FillRect(	pDD->hDC,	&rc, hFonKlap );
  else*/
	  FillRect(	pDD->hDC,	&rc, hFonSens );
	HFONT Old = (HFONT)SelectObject ( pDD->hDC, pDD->hFont );
	int OldCol = SetTextColor ( pDD->hDC, RGB ( 0, 0, 0 ));
  SetTextAlign ( pDD->hDC, TA_CENTER|TA_BASELINE );
	TextOut ( pDD->hDC, x + 72, y + 16, Name, (int)strlen ( Name ));   
  //DrawText ( pDD->hDC, Name, (int)strlen( Name), &rc,
		//  DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	SelectObject ( pDD->hDC, Old );
	SetTextColor ( pDD->hDC, OldCol );
	return false;
	}
