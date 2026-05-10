#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <SR_DCU.h>

static CY_Create ALM_R ( "ALM-R", CY_ALM_R::Create );

CY_ALM_R::CY_ALM_R( CALM_R * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  memset( representAlarms, 0, sizeof(representAlarms) );
}

bool CY_ALM_R::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "ALMK", 4) )
  {
    int i = atoi( ParmValue );
    ASS( i>=1 && i<=16);
    i--;
    SRepresentativeAlarm &ra = representAlarms[i];
    char *pDP = strchr( ParmValue, ':' );
    pDP++;
    i = atoi( pDP );
    ASS( i>=1 && i<=4);
    ra.RangeOfAlarmProcessing = i;
    i = 0;
    for( pDP = strchr( pDP+1, ':' ); pDP; pDP = strchr( pDP+1, ':' ), i++ )
    {
      char SV = 0;
      switch( *(pDP+1) )
      {
      case '.':
        SV = 0;
        break;
      case 'Y':
        SV = 1;
        break;
      case 'N':
        SV = 2;
        break;
      default:
        ASS(0);
        continue;
      }
      ra.SV[i] = SV;
    }
    KKK();
  }
  return CY_Base::ReadParm( ParmName, ParmValue, pData );
}

bool CY_ALM_R::SaveParms( HANDLE hFile )
{
  DWORD Lw;
  WRT_DATA( sizeof(representAlarms), representAlarms );
  return false;
}

bool CY_ALM_R::RestParms( HANDLE hFile )
{
  int L = 0;
  DWORD Lr;
  READ_DATA( sizeof(representAlarms), representAlarms );
  return false;
}

//////////////////////////////////////////////////////////////////////////
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ALM_R,CALM_R,30,50)

void CY_ALM_R::InitParm()
{
#include "Yokogawa\ALM-R_P.hpp" 
#include "Yokogawa\ALM-R.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_ALM_R::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
}

void CY_ALM_R::Init( )
{
}

int CY_ALM_R::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\ALM-R_P.hpp" 
  return 0;
}

int CY_ALM_R::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  return 0;
}

int CY_ALM_R::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->SV, "#SV" );
  PARM ( W->SW, "#SW" );
    MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ALM-R_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\ALM-R.hpp"
    E_MENU("Все состояние")
    MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}
