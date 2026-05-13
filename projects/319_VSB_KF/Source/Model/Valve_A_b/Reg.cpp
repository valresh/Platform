#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "Param.h"

KlapReg::KlapReg( class CValve_A_b * pMain )
{
  pKlap = pMain;
  Reg_On = false;//, "Включен" )
  Reg_PV_ = 0.;//, "#PV=" )
  Reg_MV_ = 0.;//, "#MV=" )
  Reg_PV = 0;//, "PV", "Массовый расход;Объемный расход н.у.;Объемный расход;Давление на входе;Давление на выходе;Ссылка;")
  CLEAR(Reg_Ref);//, "Ссылка", "" )
  Reg_SP = 0.;//, "Задание" )
  Reg_P = 1000.;//, "P", 1000. )
  Reg_I = 50.;//, "I", 50. )
  Reg_Dir = true;//, "Прямой", true )
  Reg_SH = 10.;//, "Масштаб PV", 10. )
  Reg_MH = 100.;//, "Масштаб MV", 100. )
  Reg_Tau = 0.;//, "Инерционность PV, c", 0. )
  Reg_Vent = true;//, "Крутить вентиль", true )
  pRef = NULL;
  /////////////////////////////
  //
  double PV;
  double En, En1;
}

void KlapReg::Reg(double dt)
{
  if ( !Reg_On )
    return;
// PV
  double pv;
    switch( Reg_PV )
      {
        case 0: // Массовый расход;
          pv = pKlap->_Flow_Work_kg;
          break;
        case 1: // Объемный расход н.у.;
          pv = pKlap->_Flow_Work_nm3;
          break;
        case 2: // Объемный расход;
          pv = pKlap->_Flow_Work_m3;
          break;
        case 3: // Давление на входе;
          pv = pKlap->_P_In;
          break;
        case 4: // Давление на выходе;
          pv = pKlap->_P_Out;
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
    MV = pKlap->Task;
  else
    MV = pKlap->K_Omega;
  MV += dMV;
  Reg_dMV_ = dMV;
  if ( MV < ML )
    MV = ML;
  if ( MV > Reg_MH )
    MV = Reg_MH;
  if ( Reg_Vent )
    pKlap->Task = MV;
  else
    pKlap->K_Omega = MV;
  Reg_MV_ = MV;
  }

double * KlapReg::GetRef ( char * Ref )
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

#define this pMain
#include "IO_Parms.h"
void KlapReg::GetParams( IBaseModel * pMain )
{
  PARM( Reg_On, "Включен" )
  PARM( Reg_PV_, "#PV=" )
  PARM( Reg_MV_, "#MV=" )
  PARM_L( Reg_PV, "PV", "Массовый расход;Объемный расход н.у.;Объемный расход;Давление на входе;Давление на выходе;Ссылка;")
  PARM( Reg_Ref, "Ссылка")
  PARM( Reg_SP, "Задание" )
  PARM( Reg_P, "P" )
  PARM( Reg_I, "I" )
  PARM( Reg_Dir, "Прямой" )
  PARM( Reg_SH, "Масштаб PV" )
  PARM( Reg_MH, "Масштаб MV" )
  PARM( Reg_Tau, "Инерционность PV, c" )
  PARM( Reg_Vent, "Крутить вентиль" )
  PARM( Reg_dMV_, "dMV=" )
}

void KlapReg::UpdateParam( CParams & Param )
{
  if ( Param.Addr == Reg_Ref )
    pRef = NULL;
}


