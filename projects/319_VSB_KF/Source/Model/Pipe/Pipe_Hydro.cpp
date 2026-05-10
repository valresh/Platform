#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
//#include "..\Drainage\Drainage.h"
#ifdef LINUX
#else
#include "TestNodes.h"
#include "HydroStruct.h"
#include "Start.h"
#endif


#undef TypeF
#undef Point


#include "SetMatr.h"

int CPipe::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  ASS(0)
  return 0;
  }

static double RT = RP * ( 20. + TK );


int CPipe::SetMatr( void * pExternals, int PntGroup,  double dt, 
                   double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( pEtalonComp && EtalonToOut > 0 )
    {
    if ( EtalonToOut == 2 && nEtalonFlow >= 0 )
      {
      A[nEtalonFlow + 1] = 1.; 
      double F = -pEtalonComp->F_et_mol;
      if ( Type[nEtalonFlow] == '+' )
        F = -F;
      B[0] = F;
      IsMF = false;
      return 0;
      }
    A[P_0] = 1.; 
    B[0] = pEtalonComp->P_et + 1.;
    IsMF = false;
    return 0;
    }
  if ( Reg_P.IsOn() )
    {
    A[P_0] = 1.; 
    B[0] = Reg_P + 1.;
    IsMF = false;
    return 0;
    }
  if ( IsMF )
    {
    memcpy ( A, Matr, ( kF + 1 ) * sizeof(double) );
    }
  else
    {
    for ( int n = 0; n < kF; n++ )
      {
      if ( Type[n] == '+' )
        A[n + 1] = 1.;
      else
        A[n + 1] = -1.;
      }
    memcpy ( Matr, A, ( kF + 1 ) * sizeof(double) );
    IsMF = true;
    }
  double O = Omega;
  if ( Razgerm )
    O = -Omega_Razgerm;
//      Vol = 0.;
  if ( O != 0. || Vol > 0. )
    {
    double Eta = -Vol / ( RT * dt );
    A[P_0] = -( O + Eta ); 
    B[0] = -( O + Eta * P );
    }
  return 0;
  }

#include "SetVar.h"

extern char Alarm_Pipe[128];

int CPipe::SetVar( void * pExternals, int PntGroup, double dt, 
                  // Переменные на входах
                  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  if ( pEtalonComp && EtalonToOut == 2 )
    KKK();
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      P = Var[P_0];
//      if ( P < P_Test )
//        KKK();
      _P = P - 1.;
//      _P_Pa = _P * Pascal;
      for ( int n = 0; n < kF; n++ )
        {
        pFlow[n] = (CFlow*)pVar[n];
        }
#ifdef SPECIAL
      if ( _Test && Test_Flow_mol )
        {
        double delta = (double)_Test * 0.01;
        for ( int n = 0; n < kF; n++ )
          {
          if ( pFlow[n] )
            {
            if ( fabs ( Test_Flow_mol[n] - pFlow[n]->Flow_mol ) > delta * fabs ( Test_Flow_mol[n] ) )
              {
              Alarm_Flow_mol = true;
              lstrcpy ( Alarm_Pipe, ObjName.Str );
              SetPause( );
              }
            }
          }
        }
#endif
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

//CFiltrFlow::CFiltrFlow()
//  {
//  memset ( this, 0, sizeof ( *this ));
//  }
