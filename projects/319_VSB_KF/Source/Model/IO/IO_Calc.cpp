#include "stdafx.h"
#include "IO.h"
#include "Err.h"
#include "CommProc.h"



void CIO::Flow_In ( double dt, CFlow * pFlow )
  {
  SET_BP BreakPoint;
  Reg_In();
  CComp * pCompIn = pFlow->Get_Comp();
  if ( kStep % 10 == 0 )
    {
    State.PH( pFlow );
    _Mw = Mw( pCompIn );
    if ( H_SmokePipe > 0. && ( Type == In || Type == Atm ))
      {
      //k_SmokePipe
      double dP = H_SmokePipe * 29e-4 / ( RP * ( T_air + TK ));
      double dP_Pipe = H_SmokePipe * _Mw * 1e-4  / ( RP * ( State.T + TK ));
      Pizb += 0.1 * ( -dP + dP_Pipe - Pizb );
      }
    }
  StateDir = -1;
  P_in = pFlow->P;
  _P_in = P_in - 1.;
  T_in = State.T;
  _Eps = State.E;
  Flow_mol_in_out = pFlow->Flow_mol;
  Flow_kg_in_out = Flow_mol_in_out * _Mw;
  Flow_m3_in_out = Flow_mol_in_out * M3;
  h_Flow_src = pFlow->h_Flow;
//
  }

void CIO::Flow_Out ( double dt, CFlow * pFlow )
  {
  SET_BP BreakPoint;
//  CompZ.Set ( pComp );
  Reg_Out();
  if ( kStep % 10 == 0 || _Mw == 0. )
    {
    Set_PTE( );
    _Mw = Mw_src = ::Mw ( pComp );
    }
  _Eps = State.E;
  h_Flow_src = State.h;
  T_src = State.T;
  pFlow->ID_COMP = ID_Comp;
  pFlow->pComp = pComp;
  pFlow->h_Flow = h_Flow_src;
  pFlow->T = State.T;
  pFlow->To_kg = Mw_src;
//
  P_in = pFlow->P;
  _P_in = P_in - 1.;
  T_in = T;
  Flow_mol_in_out = pFlow->Flow_mol;
  Flow_kg_in_out = Flow_mol_in_out * _Mw;
  Flow_m3_in_out = Flow_mol_in_out * M3;
  h_Flow_src = pFlow->h_Flow;
  }

void CIO::Calc( double dt )
	{
  SET_BP BreakPoint;
  //if ( Script.kScript > 0 )
  //  {
  //  Script.Work();
  //  }
  switch ( Type )
    {
    case IO:
      {
      if ( pFlow_in == NULL || pFlow_out == NULL )
        return;
      if ( WorkAsSrc )
        {
        if ( pFlow_in->Flow_mol >= 0. )
          Flow_In ( dt, pFlow_in );
        if ( pFlow_out->Flow_mol >= 0. )
          Flow_Out ( dt, pFlow_out );
        return;
        }
      CFlow * pF; 
      if ( pFlow_in->Flow_mol > 0. )
        {
        pF = pFlow_in;
        *pFlow_out = *pFlow_in;
        }
      else
        {
        pF = pFlow_out;
        *pFlow_in = *pFlow_out;
        }
      if ( fabs ( pF->Flow_mol ) > 1e-4 )
        {
        if ( kStep % 10 == 0 )
          State.PH( pF );
        }
      else
        {
        State.T = T_air;
        State.E = 0.;
        }
      P_in = pF->P;
      _P_in = P_in - 1.;
      T_in = State.T;
      _Eps = State.E;
      _Mw = Mw( State.pComp );
      Flow_mol_in_out = pF->Flow_mol;
      Flow_kg_in_out = Flow_mol_in_out * _Mw;
      Flow_m3_in_out = Flow_mol_in_out * M3;
      h_Flow_src = pF->h_Flow;
      }
    return;
    case In:
    case Atm:
      {
      if ( pFlow_in )
        {
        if ( pFlow_in->Flow_mol >= 0. )
          Flow_In ( dt, pFlow_in );
        else
          Flow_Out ( dt, pFlow_in );
        }
      }
    return;
    case Out:
      {
      if ( pFlow_out )
        {
        if ( pFlow_out->Flow_mol >= 0. )
          Flow_Out ( dt, pFlow_out );
        else
          Flow_In ( dt, pFlow_out );
        }
      }
    return;
    default:
//      ASS(0)
    break;
    }
	}

void CIO::SetComp( )
  {
  if ( Type != Out && !WorkAsSrc )
    return ;
  SET_BP BreakPoint;
  ReadType( );
  memcpy( Cmol, pComp->Cmol, sizeof (Cmol));
  memcpy(IsComp, pComp->IsComp,sizeof (IsComp));
//  CompZ.Set ( pComp );
  //
  _Mw = Mw_src = ::Mw ( pComp );
  Set_PTE( );
  }

void CIO::ReadType( )
  {
  switch( CompDefect )
    {
    default:
    case 0:
      if ( Composition[0] )
        pComp->Read ( Composition );
      else
        pComp->Set ( pN2 );
    break;
    case 1:
      if ( Composition_Defect[0] )
        pComp->Read ( Composition_Defect );
      else
        pComp->Set ( pN2 );
    break;
    }
  double N = Norm( pComp );
  }

void CIO::SavePTC( )
  {
  CFlow * pF; 
  if ( pFlow_in && pFlow_in->Flow_mol > 0. )
    pF = pFlow_in;
  else
    pF = pFlow_out;
  if ( pF == NULL )
    return;
  State.PH( pF );
  P_in = pF->P;
  T_in = State.T;
  Flow_mol_in_out = pF->Flow_mol;
  CComp * pComp = pF->Get_Comp();
  Flow_kg_in_out = Flow_mol_in_out * Mw ( pComp );
  Char<512>Path;
  Path.Prt ( "%sData\\IO\\%s.txt", PROJECT_ROOT, ObjName.Str );
  TakeOffReadOnly( Path );
  void * F = Fopen ( Path, "wt" );

  if ( F == NULL )
    {
    MsgErr ( NULL, "Ошибка записи состояния %s", ObjName.Str );
    return;
    }
  Fprintf ( F, "%8.6lf;P(а)\n", P_in );
  Fprintf ( F, "%8.6lf;T\n", T_in );
  Fprintf ( F, "%8.6lf;F(кг)\n", Flow_kg_in_out );
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( pComp->IsComp[c] )
      {
      Fprintf ( F, "%9.7lf;%s\n", pComp->Cmol[c], pGas[c]->Name );
      }
    }
  Fclose ( F );
  }

bool CIO::RestPTC( )
  {
  Char<512>Path;
  Path.Prt ( "%sData\\IO\\%s.txt", PROJECT_ROOT, ObjName.Str );
  void * F = Fopen ( Path, "rt" );
  if ( F == NULL )
    {
    ASS(0)
    return false;
    }
  char Str[256];
  Fgets( Str, 255, F );
  P_in = atof( Str );
  Fgets( Str, 255, F );
  T_in = atof( Str );
  Fgets( Str, 255, F );
  Flow_kg_in_out = atof( Str );
  Reg_Flow_Z_kg.Value = Flow_kg_in_out;
//
  CLEAR(IsComp)
  CLEAR(Cmol)
  while( Fgets( Str, 255, F ) )
    {
    int L = strlen ( Str ) - 1;
    Str[L] = 0;
    char * C = Str;
    char * Name = strchr ( C, ';' );
    if ( Name == 0 )
      {
      ASS(0)
      return false;
      }
    Name++;
    bool OK = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( lstrcmp ( pGas[c]->Name, Name ) == 0 )
        {
        OK = true;
        IsComp[c] = true;
        Cmol[c] = atof ( C );
        break;
        }
      }
    if ( !OK )
      {
      ASS(0)
      return false;
      }
    }
//
  Pizb = P_in - 1.;
  T = T_in; 
  pComp->Set( IsComp, Cmol );
  Composition[0] = '*';
  Composition[1] = 0;
//  CompZ.Set ( pComp );
  //
  _Mw = Mw_src = ::Mw ( pComp );
  Set_PTE( );
  return true;
  }

void CIO::Set_PTE(  )
  {
  switch ( Параметры )
    {
    case 0: // P+T
      {
      State.PT( Pizb + 1., T, pComp );
      _Eps = Eps = State.E;
      h_Flow_src = State.h;
      T_src = State.T;
      }
    break;
    case 1: // P+E
      {
      State.PE( Pizb + 1., Eps, pComp );
      _Eps = State.E;
      h_Flow_src = State.h;
      T_src = State.T;
      }
    break;
    case 2: // T+E;
      {
      State.TE( T, Eps, pComp );
      Pizb = State.P - 1.;
      _Eps = State.E;
      h_Flow_src = State.h;
      T_src = State.T;
      }
    break;
    }
  }
