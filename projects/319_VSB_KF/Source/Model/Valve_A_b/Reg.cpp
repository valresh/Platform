#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "Param.h"


void CCV::Reg(double dt)
  {
  if ( !Reg_On )
    return;
// PV
  double pv;
  switch( Reg_PV )
    {         
    case 0: // Массовый расход;
      pv = _Flow_Work_kg;
    break;
    case 1: // Объемный расход н.у.;
      pv = _Flow_Work_nm3;
    break;
    case 2: // Объемный расход;
      pv = _Flow_Work_m3;
    break;
    case 3: // Давление на входе;
      pv = _P_In;
    break;
    case 4: // Давление на выходе;
      pv = _P_Out;
    break;
    case 5: // Ссылка;
      {
      if ( Reg_Ref[0] == 0 )
        {
        pv = 0.;
        strcpy( Reg_Ref, "?" );
        break;
        }
      if ( pRef == NULL )
        {
        pRef = GetRef ( Reg_Ref );
        if ( pRef == NULL )
          {
          if ( strchr ( Reg_Ref, '?' ) == NULL )
            strcat ( Reg_Ref, "?" );
          pv = 0.;
          break;
          }
        }
      pv = *pRef;
      }
    break;
    }
//
  dt *= 3600.;
  if ( PV < 0. )
    PV = pv;
  else
    {
    if ( Reg_Tau == 0. )
      PV = pv;
    else
      {
      double Ksi = dt / Reg_Tau;
      PV = ( PV + Ksi * pv ) / ( 1. + Ksi );
      }
    }
  Reg_PV_ = PV;
//
  En1 = En;  
  En = PV - Reg_SP;
//
  const double ML = 0.;
  const double SL = 0.;
  double Ks = ( Reg_MH - ML ) / ( Reg_SH - SL );
  double Kp = 100. / Reg_P;
  double dEn = En - En1;
  
  double dMV;
  if ( Reg_I > 0.01 )
    dMV = Kp * Ks * ( dEn + dt / Reg_I * En );
  else
    dMV = Kp * Ks * dEn;
  if ( Reg_Dir )
    dMV = -dMV;
  if ( dMV > 0.05 * Reg_MH )
    dMV = 0.05 * Reg_MH;
  if ( dMV < -0.05 * Reg_MH )
    dMV = -0.05 * Reg_MH;
//
  double MV;
  if ( Reg_Vent )
    MV = Position;
  else
    MV = K_Omega;
  MV += dMV;
  Reg_dMV_ = dMV;
  if ( MV < ML )
    MV = ML;
  if ( MV > Reg_MH )
    MV = Reg_MH;
  if ( Reg_Vent )
    Position = Task = MV;
  else
    K_Omega = MV;
  Reg_MV_ = MV;
  }

double * CCV::GetRef ( char * Ref )
  {
  char Name[256];
  strcpy_s ( Name, 256, Ref );
  char * P = strchr ( Name, '.' );
  if ( P == NULL )
    return NULL;
  *P = 0;
  P++;
  IBaseModel * pModel = IBaseModel::Find ( Name );
  if ( pModel == NULL )
    return NULL;
  CLockParams Lock;
  pModel->GetParams ( "" );
  int K = CParams::kParams;
  for ( int n = 0; n < K;n++ )
    {
    if ( strcmp ( CParams::Params[n].ParamName, P ) == 0 )
      {
      if ( CParams::Params[n].Type == 'D' )
        {
        return (double*)CParams::Params[n].Addr;
        }
      break;
      }
    }
  return NULL;
  }

int CCV::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == Reg_Ref )
    pRef = NULL;
  return 0;
  }
