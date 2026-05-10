#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"

CPseudo_Water::CPseudo_Water( IBaseModel * pMain )
  {
  memset ( this, 0, sizeof ( *this ));
  Hydro_Water = 0.;
  Level_Water = 0.;
  Omega_Water = 100.;
  L_WaterZ = 0.;
  Max_Water = 100.;
  Eps_Water = 0.01;
  pLev = new CAnalogD ( pMain, "Уровень воды", "AO");
  }

int CPseudo_Water::GetParams( IBaseModel * pMain )
  {
#define this pMain
#include "IO_Parms.h"
  PARM( M_Water, "#Масса воды, кг" );
  PARM( Level_Water, "#Уровень воды, %" );
  PARM( Hydro_Water, "Гидростат воды (от уровня)" );
  PARM( Omega_Water, "Проходимость выхода воды" );
  PARM( L_WaterZ, "Заданный уровень воды, %" );
  PARM( Max_Water, "Максимальная масса воды, кг" );
  PARM( Eps_Water, "Мольная доля воды в потоки, %" );
#undef this
  return 0;
  }

int CPseudo_Water::SaveState ( )
  {
  S_CLASS("Pseudo_Water", CPseudo_Water_W )
  return 0;
  }

int CPseudo_Water::RestoreState ( char * StrName )
  {
  R_CLASS("Pseudo_Water", CPseudo_Water_W )
  return 1;
  }

void CPseudo_Water::Calc( double dt, CFlow * pFlow )
  {
  if ( pFlow == NULL )
    return;
  double F = fabs ( pFlow->Flow_mol );
  double F_W_kg = F * Eps_Water * 0.18;
  M_Water += dt * F_W_kg;
  if ( M_Water > Max_Water )
    M_Water = Max_Water;
  if ( L_WaterZ > 0. )
    {
    M_Water = Max_Water * L_WaterZ * 0.01;
    L_WaterZ = 0.;
    }
  if ( L_WaterZ < 0. )
    {
    M_Water = -Max_Water * L_WaterZ * 0.01;
    }
  Level_Water = M_Water / Max_Water * 100.;
  *pLev = Level_Water;
  }

void CPseudo_Water::Out( double dt, double T, CFlow * pFlowWater )
  {
  double F = pFlowWater->Flow_mol;
  if ( F > 0. )
    {
    double F_W_kg = F * 18.;
    M_Water += dt * F_W_kg;
    if ( M_Water > Max_Water )
      M_Water = Max_Water;
    }
  else
    {
    double F_W_kg = F * 18.;
    M_Water += dt * F_W_kg;
    if ( M_Water < 0. )
      M_Water = 0.;
    pFlowWater->ID_COMP = ID_Water;
    pFlowWater->h_Flow = pGas[G_H2O]->H_l( T );
    pFlowWater->pComp = pWater;
    pFlowWater->T = T;
    }
  Level_Water = M_Water / Max_Water * 100.;
  }

void CPseudo_Water::Matr( double P, double Mp[], double Mf[], double V[] )
  {
  Mp[0] = Omega_Water;
  Mf[0] = -1.;
  V[0] = Omega_Water * P;
  }
