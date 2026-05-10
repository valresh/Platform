#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "ConnectY.h"
#include "ReadParms.h"
#include "GetObj.h"
 
static CY_Create AREAINOUT_IN ( "AREAIN", CY_AREAIN::Create, CY_AREAIN::AREAIN );
static CY_Create AREAINOUT_OUT ( "AREAOUT", CY_AREAIN::Create, CY_AREAIN::AREAOUT );

CY_AREAIN::CY_AREAIN( char * _Name, int _Type, int number, int domain ) : CY_Base()
{
  Type = (eType)_Type; 
	PV = 0.;
	iPV = 0;
	OUT.Name = Name;
	OUT.Null();
	IN = NULL;
	pFound = &Is;
	Is = false;
  GAIN = 1.;
	BIAS = 0.;
  CLEAR(Ref)
  pRefVar = NULL;
  TypeRefVar = ' ';
  m_pObjX = NULL;
}

#include "Yokogawa\YPARM_INIT.h" 
#define MAX_VARS 1
static CY_AREAIN_W _W;
static CYVarInfo VarInfo[MAX_VARS];
static int kVarInfo = 0;

void CY_AREAIN::SetParmList( )
  {
  ClassParmInfo = NULL;
  kClassParmInfo = 0;
  ClassVarInfo = VarInfo;
  kClassVarInfo = kVarInfo;
  pClassBase = (BYTE*)static_cast<CY_AREAIN_W*>(this);
  pClass_P = NULL;
  L_Class_P = 0;
  }

void CY_AREAIN::InitParm()
{
  kVarInfo = 0;
  DOUBLE(PV,"")
  VarInfo[0].Work = 4;
}

CSignal * CY_AREAIN::GetOUT ( char * Name )
	{
//??	ASS ( strcmp ( Name, "OUT" ) == 0 )
  return &OUT;
	}

CSignal ** CY_AREAIN::GetIN ( char * Name )
	{
  //ASS(FALSE)
	//ASS ( strcmp ( Name, "IN" ) == 0 )
	return &IN;
	}

void CY_AREAIN::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
  if ( IN )
  {
    // Входной сигнал -> PV
    if( IN->Type == CSignal::Int )
    {
      *IN >> iPV;
    }
    else
    {
      *IN >> PV;
    }
    if( pRefVar )
    {
      switch( TypeRefVar )
      {
      case 'W':
        *(BYTE*)pRefVar = iPV;
        break;
      case 'I':
        *(int*)pRefVar = iPV;
        break;
      case 'D':
        *(double*)pRefVar = PV;
        break;
      }
    }
    return;
  }
  if( !pRefVar )
    return;
  switch( TypeRefVar )
  {
  case 'W':
    iPV = *(BYTE*)pRefVar;
    break;
  case 'I':
    iPV = *(int*)pRefVar;
    break;
  case 'D':
    PV = *(double*)pRefVar;
    break;
  }
}

void CY_AREAIN::Init( )
	{
  ClassVarInfo = VarInfo;
  kClassVarInfo = kVarInfo;
  pClassBase = (BYTE*)(static_cast<CY_AREAIN_W*>(this));
	}

int CY_AREAIN::Prepare0()
{
  Is = false;
// Сcылка -> на выход
	if( Name[0] == '%' && Name[1] == '%'	)
		{
    KKK();
		/*Is = false;
		return 0;*/
		}
	strcpy_s( Ref, Name );
  
  bool bRefToOtherFcs = false;
  if( '>'==Name[0] )
  {
    strcpy_s( Ref, &Name[1] );
    bRefToOtherFcs = true;
  }

  char * P = strchr ( Ref, '.' );
  if( P )
    *P = 0;
  else
    KKK();
  m_pObjX = GetObj( Ref, this, bRefToOtherFcs );

  if( !m_pObjX )
    m_pObjX = (*pFindYObjN)( Ref, bRefToOtherFcs ? (-pDR->NumbFCS) : pDR->NumbFCS, pDR->domain );
  if( !m_pObjX && Type==CY_AREAIN::AREAOUT )
  {
    m_pObjX = (*pFindYObjN)( Ref, -pDR->NumbFCS, pDR->domain );
    ASS( m_pObjX!=this );
  }
  if( m_pObjX==this )
    m_pObjX = pFindDObj( Ref, pDR->NumbFCS, pDR->domain );
  
  if( !m_pObjX && Type==CY_AREAIN::AREAOUT && !bRefToOtherFcs )
  {
    m_pObjX = (*pFindYObjN)( Ref, pDR->NumbFCS, -pDR->domain );
    if( m_pObjX )
    {
      char szBuf[256];
      sprintf_s( szBuf, "AREAOUT нашла точку \"%s\" в другом домене\n", Ref );
      OutputDebugString( szBuf );
    }
  }

  if( m_pObjX == NULL )
  {
#pragma message("много не существующих точек")
    //ASS( pObj );
    return 0;
  }

  {
    CAlarmBase::tFlagUse_ *pFL = m_pObjX->GetFlagUse();
    //ASS( pFL );
    if( pFL )
      CY_Base::InitFlagUse( *pFL );
  }
  void * pTuneVar = NULL;
  CY_Base::eVarType Work = CY_Base::evtP;
  bool Res = m_pObjX->GetVar ( P ? P + 1 : "PV", &pRefVar, &pTuneVar, &TypeRefVar, &Work );
  if ( !Res )
    {
    return 0;
    }
  Is = true;
  if( id__Z==m_pObjX->Y_Type)
  {
    OUT.On( CSignal::Z_Connection );
    ASSD( 'Z'==m_pObjX->Model[0] && 0==m_pObjX->Model[1] );
  }
  if( id__Y==m_pObjX->Y_Type)
  {
    OUT.On( CSignal::Z_Connection );
    ASSD( 'Y'==m_pObjX->Model[0] && 0==m_pObjX->Model[1] );
  }
  switch ( TypeRefVar )
  {
  case 'W':
    if( pRefVar )
      OUT << *(BYTE*)pRefVar;
    else
      OUT << iPV;
    break;
  case 'I':
    if( pRefVar )
      OUT << *(int*)pRefVar;
    else
      OUT << iPV;
  break;
  case 'D':
    if( pRefVar )
      OUT << *(double*)pRefVar;
    else
      OUT << PV;
    break;
  break;
  default:
    ASS(FALSE)
  break;
  }
	return 0;
}

int CY_AREAIN::Prepare1()
{
	return 0;
}

int CY_AREAIN::SaveState( )
{
	CY_Base::SaveState( );
	OUT.SaveState();
	SAVE_CLASS("W",CY_AREAIN_W)
	return 0;
}

int CY_AREAIN::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_AREAIN_W)
	return 1;
}

int CY_AREAIN::StateSave( IYokoStateSer* pStateSer )
{
  //OUT.StateSave( pStateSer );
  WriteFullStruct( pStateSer, eKeyAnyVar, "PV", PV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "iPV", iPV );
  return 0;
}

int CY_AREAIN::StateRestore( IYokoStateSer* pStateSer )
{
  //OUT.StateRestore( pStateSer );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "PV", PV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "iPV", iPV );
  return 0;
}

int CY_AREAIN::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  switch ( TypeRefVar )
    {
    case 'I':
		  {
		  PARM ( iPV, "#iPV" );
		  }
    break;
    case 'D':
  		{
	  	PARM ( PV, "#PV" );
		  }
    break;
    }
	PARM ( Is, "#Найден" );
  PARM ( GAIN, "GAIN" );
	PARM ( BIAS, "BIAS" );	
	MENU("Base")
	  CY_Base::GetParams( params );
	E_MENU("Base")
	MENU("Входы")
	PARM_IN(IN)
	PARM_OUT
	E_MENU("Входы")
	return 0;
	}

bool CY_AREAIN::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  if( !m_pObjX )
    Prepare0();
  bool bCommon = __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );

  bool bPV = /*VarName[0] == 0 ||*/ ( VarName[0] == 'P' && VarName[1] == 'V' && VarName[2] == 0);
  ASS( bPV );
  switch ( TypeRefVar )
  {
  case 'W':
  case 'I':
  case 'D':
    *pVar = pRefVar;
    *Type = TypeRefVar;
    break;
  default:
    break;
  }
  return true;
}
