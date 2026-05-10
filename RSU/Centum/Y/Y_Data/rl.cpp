#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/AutoRestore.h>

static CY_Create RL ( "RL", CY_RL::Create );

CY_RL_W::CY_RL_W()
	{
	memset ( this, 0, sizeof ( CY_RL_W ));
	}

CY_RL::CY_RL( CRL * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CLEAR(L)
  CLEAR(R)
  CLEAR(Q)
	}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_RL,CRL,200,250)

void CY_RL::InitParm()
  {
#include "Yokogawa\RL_P.hpp" 
#include "Yokogawa\RL.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

bool CY_RL::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if ( strcmp ( ParmName, "RELN" ))
    return CY_Base::ReadParm( ParmName, ParmValue, pData );
  int &kRef = W->kRef;
  ASS(kRef<16)
  char Fld[30][64];
  int K = SetFld_X( ParmValue, ':', Fld );
  if ( Fld[1][0] == 0 )
    {
    if ( Fld[2][0] == 0 )
      return true;
    if ( Fld[3][0] == 0 )
      return true;
    sprintf_s ( W->Ref[kRef], "@%d", 2 * kRef );
    }
  else
    strcpy_s ( W->Ref[kRef], Fld[1] );
  if( !strcmp( Fld[2], "CMP" ) )
    W->Oper[kRef] = eCMP;
  else if( !strcmp( Fld[2], "AND" ) )
    W->Oper[kRef] = eAND;
  else
  {
    W->Oper[kRef] = eHZ;
  }
  //bool Tag = Fld[3][0] == '4' && Fld[3][1] == '8' && isalpha ( Fld[3][2] );
  bool Tag = false;
  for( size_t n=0, e(strlen(Fld[3])); n<e; ++n )
  {
    if( isalpha(Fld[3][n]) )
    {
      Tag = true;
      break;
    }
  }
	if ( Tag /*|| isalpha ( Fld[3][0] )*/ )
  {
		W->UC[kRef] = 0;
		strcpy_s( W->D[kRef], Fld[3] );
    for( LPSTR p=W->D[kRef]; *p; ++p )
      *p = toupper(*p);
	}
	else
	{
		W->UC[kRef] = 1;
    W->C[kRef] = atof ( Fld[3] );
	}
  kRef++;
  W->kRef = kRef;
  return true;
}

CSignal * CY_RL::GetOUT ( char * Name )
	{
  //ASS(FALSE)
	return NULL;
	}

CSignal ** CY_RL::GetIN ( char * Name )
	{
  GET_IN_NAME(Q[0],"Q01")
  GET_IN_NAME(Q[1],"Q02")
  GET_IN_NAME(Q[2],"Q03")
  GET_IN_NAME(Q[3],"Q04")
  GET_IN_NAME(Q[4],"Q05")
  GET_IN_NAME(Q[5],"Q06")
  GET_IN_NAME(Q[6],"Q07")
  GET_IN_NAME(Q[7],"Q08")
  GET_IN_NAME(Q[8],"Q09")
  GET_IN_NAME(Q[9],"Q10")
  GET_IN_NAME(Q[10],"Q11")
  GET_IN_NAME(Q[11],"Q12")
  GET_IN_NAME(Q[12],"Q13")
  GET_IN_NAME(Q[13],"Q14")
  GET_IN_NAME(Q[14],"Q15")
  GET_IN_NAME(Q[15],"Q16")
  GET_IN_NAME(Q[16],"Q17")
  GET_IN_NAME(Q[17],"Q18")
  GET_IN_NAME(Q[18],"Q19")
  GET_IN_NAME(Q[19],"Q20")
  GET_IN_NAME(Q[20],"Q21")
  GET_IN_NAME(Q[21],"Q22")
  GET_IN_NAME(Q[22],"Q23")
  GET_IN_NAME(Q[23],"Q24")
  GET_IN_NAME(Q[24],"Q25")
  GET_IN_NAME(Q[25],"Q26")
  GET_IN_NAME(Q[26],"Q27")
  GET_IN_NAME(Q[27],"Q28")
  GET_IN_NAME(Q[28],"Q29")
  GET_IN_NAME(Q[29],"Q30")
  GET_IN_NAME(Q[30],"Q31")
  GET_IN_NAME(Q[31],"Q32")
  ASS(FALSE)
	return NULL;
	}

bool CY_RL::CallSA(LPCSTR pszParam)
{
  LPCSTR pX = strchr( pszParam, '.' );
  LPCSTR pRel = strrchr( pszParam, '.' );
  ASS(pX);
  ASS(pRel);
  if( pRel==pX )
    pX = pszParam;
  if( 'X'==pX[0] )
    pX += 1;
  else if( 'X'==pX[1] )
    pX += 2;
  else
  {
    ASS(0);
    return false;
  }
  int n = atoi( pX );
  if( n<1 || n>16 )
  {
    ASS(0);
    return false;
  }
  --n;

  int &X = *(&(W->X01) + n);

  if( Q[n*2] )
    *Q[n*2] >> W->RV[n*2];
  if( Q[n*2+1] )
    *Q[n*2+1] >> W->RV[n*2+1];

  if( eCMP==W->Oper[n] )
  {
    double L_V = W->RV[n*2];
    double R_V = W->RV[n*2+1];
    ++pRel;
    if( 'G'==pRel[0] && 'E'==pRel[1] )
    {
      if( abs((double)L_V - (double)R_V) < 0.0001 )
        X = 1;
      else
        X = L_V >= R_V ? 1 : 0;
    }
    else if( 'L'==pRel[0] && 'E'==pRel[1] )
    {
      if( abs((double)L_V - (double)R_V) < 0.0001 )
        X = 1;
      else
        X = L_V <= R_V ? 1 : 0;
    }
    else if( 'G'==pRel[0] && 'T'==pRel[1] )
    {
      if( abs((double)L_V - (double)R_V) < 0.0001 )
        X = 0;
      else
        X = L_V > R_V ? 1 : 0;
    }
    else if( 'L'==pRel[0] && 'T'==pRel[1] )
    {
      if( abs((double)L_V - (double)R_V) < 0.0001 )
        X = 0;
      else
        X = L_V < R_V ? 1 : 0;
    }
    else if( 'E'==pRel[0] && 'Q'==pRel[1] )
      X = abs(L_V - R_V) < 0.0001 ? 1 : 0;
    else
    {
      ASSD(0);
      return false;
    }
  }
  else if( eAND==W->Oper[n] )
  {
    int L_V = W->AV[n*2];
    int R_V = W->AV[n*2+1];
    x = L_V & R_V;
    KKK();
  }

  return true;
}

void CY_RL::Work ( double dt )
{
	SET_HBP
	{
		KKK();
	}
  int * X = &(W->X01);
  ASS( W->kRef <= 16 )
  int n_RL = 0;
	for ( int n = 0; n < W->kRef; n++ )
	{
    if ( L[n] == NULL )
    {
      n_RL++;
      n_RL++;
      continue;
    }
		double L_V;
    if ( TypeL[n] == 'D' )
		  L_V = *(double*)L[n];
    else
		  L_V = *(int*)L[n];
		double R_V;
		if ( W->UC[n] )
			R_V = W->C[n];
		else
		{
      if ( TypeR[n] == 'D' )
		    R_V = *(double*)R[n];
      else
		    R_V = *(int*)R[n];
		}
    if( eCMP==W->Oper[n] )
    {
      if( !Q[n_RL] )
        W->RV[n_RL] = L_V;
      else
        *Q[n_RL] >> W->RV[n_RL];
    }
    else if( eAND==W->Oper[n] )
    {
      if( Q[n_RL] )
        *Q[n_RL] >> L_V;
      W->AV[n_RL] = L_V;
    }
    n_RL++;
    if( eCMP==W->Oper[n] )
    {
      if( !Q[n_RL] )
        W->RV[n_RL] = R_V;
      else
        *Q[n_RL] >> W->RV[n_RL];
    }
    else if( eAND==W->Oper[n] )
    {
      if( Q[n_RL] )
        *Q[n_RL] >> R_V;
      W->AV[n_RL] = R_V;
    }
    n_RL++;
	}
}

void CY_RL::Init( )
	{
	}

int CY_RL::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\RL_P.hpp" 
	return 0;
	}

int CY_RL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
// Поиск ссылок
	for ( int n = 0; n < W->kRef; n++ )
	{
    if ( W->Ref[n][0] == '@' )
    {
      int N = atoi ( W->Ref[n]+1 );
      CSignal * pS = Q[N];
//      ASS(pS)
      if ( pS == NULL )
        continue;
      lstrcpy ( W->Ref[n], pS->pFrom );
      L[n] = NULL;
      if ( pS->Type == CSignal::Int )
      {
        L[n] = pS->pInt;
        TypeL[n] = 'I';
      }
      if ( pS->Type == CSignal::Double )
      {
        L[n] = pS->pDbl;
        TypeL[n] = 'D';
      }
    }
    else
    {
      TAutoRestore<char> arP;
		  char * P = strchr ( W->Ref[n], '.' );
		  if( P )
      {
        arP.Set( *P );
        *P = 0;
      }
      CY_Base * pLObj = (*pFindYObjN)( W->Ref[n], pDR->NumbFCS, pDR->domain );
      if( !pLObj )
      {
        OutputDebugString( "RL не нашел объект на своей станции ");OutputDebugString( W->Ref[n] );OutputDebugString("\n");
        continue;
      }
      void * pVar = NULL;
      void * pTuneVar = NULL; 
      char Type = ' ';
      CY_Base::eVarType Work = CY_Base::evtP;
      if( P )
      {
        ASSD( pLObj->GetVar( P + 1, &pVar, &pTuneVar, &Type, &Work ) )
      }
      else
      {
        ASSD( pLObj->GetVar( "PV", &pVar, &pTuneVar, &Type, &Work ) )
      }
      ASSD(Type == 'D'||Type == 'I');
      L[n] = pVar;
      TypeL[n] = Type;
    }
    ASS( L[n] )
		if ( W->UC[n] )
			continue;
		char * P = strchr ( W->D[n], '.' );
		ASS ( P )
		*P = 0;
    void * pVar = NULL;
    void * pTuneVar = NULL; 
    char Type = ' ';
    CY_Base::eVarType Work = CY_Base::evtP;
    CY_Base * pRObj = (*pFindYObjN)( W->D[n], pDR->NumbFCS, pDR->domain );
    ASSD( pRObj );
    if( !pRObj )
      continue;
    ASS(pRObj->GetVar ( P + 1, &pVar, &pTuneVar, &Type, &Work ))
    ASS(Type == 'D'||Type == 'I');
    R[n] = pVar;
    TypeR[n] = Type;
		*P = '.';
		ASS( R[n] )
		}
	return 0;
}

int CY_RL::SaveState( )
	{
	SAVE_CLASS("W",CY_RL_W)
	return 0;
	}

int CY_RL::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;

//	REST_CLASS("W",CY_RL_W)
	return 1;
	}

int CY_RL::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Условия")
	char Name[256];
	for ( int n = 0; n < W->kRef; n++ )
		{
		if ( W->UC[n] )
			{
      if ( TypeL[n] == 'I' )
			  sprintf_s ( Name, "#%s[X%02d]%d", W->Ref[n], n + 1, (int)W->C[n] );
      else
			  sprintf_s ( Name, "#%s[X%02d]%4.2lf", W->Ref[n], n + 1, W->C[n] );
			PARM ( W->RV[2*n], Name );
			}
		else
			{
			sprintf_s ( Name, "#%s[X%02d]%s", W->Ref[n], n + 1, W->D[n]	);
			PARM ( W->RV[2*n], Name );
			}
		}
	E_MENU("Условия")
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\RL_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\RL.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_RL::SetLimits( )
  {
  return true;
  }
